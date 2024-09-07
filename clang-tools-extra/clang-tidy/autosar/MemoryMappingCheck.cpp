//===--- MemoryMappingCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MemoryMappingCheck.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Tooling/Tooling.h"

#define OPTION_SET_VARIABLE(Options_, Dest_, Name_)                            \
  (Options_).store((Dest_), (#Name_), (Name_))

#define OPTION_SET(Options_, Dest_, Name_, Default_)                           \
  (Options_).store((Dest_), (Name_), (Options_).get((Name_), (Default_)))

using namespace clang::ast_matchers;

namespace clang::tidy::autosar {

namespace {

constexpr StringRef MatchIDFunctionDecl = "Fn";
constexpr StringRef MatchIDVarDecl = "Var";

constexpr StringRef DefaultPolicyCleared = "CLEARED;POWER_ON_CLEARED";
constexpr StringRef DefaultPolicyInit = "INIT";
constexpr StringRef DefaultMemMapInclusion = "*_MemMap.h";
constexpr StringRef DefaultSectionCode = "CODE";
constexpr StringRef DefaultSectionConst = "CONST";
constexpr StringRef DefaultSectionVar = "VAR";
constexpr StringRef DefaultKeywordSectionStart = "_START_SEC_";
constexpr StringRef DefaultKeywordSectionStop = "_STOP_SEC_";
constexpr StringRef DefaultAlignBoolean = "BOOLEAN";
constexpr StringRef DefaultAlign8 = "8";
constexpr StringRef DefaultAlign16 = "16";
constexpr StringRef DefaultAlign32 = "32";
constexpr StringRef DefaultAlign64 = "64";
constexpr StringRef DefaultAlignPointer = "PTR";
constexpr StringRef DefaultAlignUnspecified = "UNSPECIFIED";

class CallbackManager : public PPCallbacks {
public:
  CallbackManager(MemoryMappingCheck *Check) : Check(Check) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    const auto *Info = MD->getMacroInfo();

    if (Info->isFunctionLike())
      return;

    /* Memory mapping macro keywords do not resolve to any tokens */
    if (Info->getNumTokens() != 0)
      return;

    auto Name = MacroNameTok.getIdentifierInfo()->getName();
    if (Name.empty())
      return;

    //
    // AUTOSAR Classic Platform R22-11: Specification of Memory Mapping
    // [SWS_MemMap_00022]
    //
    StringRef Prefix;
    StringRef Suffix;
    Keyword::Kind Kind = Keyword::Kind::NONE;

    for (const auto &[Key, Value] : Check->getKeywordKinds()) {
      std::tie(Prefix, Suffix) = Name.split(Key);
      if (!Suffix.empty()) {
        Kind = Value;
        break;
      }
    }

    // We don't consider this macro to be a valid memory mapping keyword if
    // it does not contain a valid start or stop affix.
    if (Kind == Keyword::Kind::NONE)
      return;

    StringRef View = Suffix;

    //
    // AUTOSAR Classic Platform R22-11: Specification of Memory Mapping
    // [SWS_MemMap_00038]
    //
    Keyword::SectionType Section = Keyword::SectionType::NONE;

    for (const auto &[Key, Value] : Check->getSectionTypes()) {
      StringRef Copy = View;

      if (!Copy.consume_front(Key))
        continue;

      if (!Copy.empty() && !Copy.consume_front("_"))
        continue;

      View = Copy;
      Section = Value;
      break;
    }

    if (Section == Keyword::SectionType::NONE)
      return;

    Keyword::InitPolicy Policy = Keyword::InitPolicy::NONE;
    Keyword::AlignSpec Alignment = Keyword::AlignSpec::NONE;

    switch (Section) {
    case Keyword::SectionType::VAR:
      // Using explicitly 'contains()' as the beginning of 'View' might not
      // point to the strings we are searching.
      // Notice how the execution order of the conditions below is crucial for
      // detecting the correct initiialization policy.

      for (const auto &[Key, Value] : Check->getInitPolicies()) {
        StringRef Copy = View;

        if (!Copy.consume_front(Key))
          continue;

        if (!Copy.empty() && !Copy.consume_front("_"))
          continue;

        View = Copy;
        Policy = Value;
        break;
      }

      if (Policy == Keyword::InitPolicy::NONE) {
        Check->diag(MacroNameTok.getLocation(),
                    "missing or invalid initialization policy in keyword '%0'")
            << Name;
      }

      LLVM_FALLTHROUGH;
    case Keyword::SectionType::CONST:
      // Check the keyword's alignment specification. Sections for constant
      // data are allowed to omit the alignment specification.

      if (Section == Keyword::SectionType::CONST && View.empty()) {
        if (!Check->strictAlignmentSpecifications()) {
          Alignment = Keyword::AlignSpec::UNSPECIFIED;
          break;
        }
      } else {
        for (const auto &[Key, Value] : Check->getAlignmentSpecs()) {
          StringRef Copy = View;

          if (!Copy.consume_back(Key))
            continue;

          if (!Copy.empty() && !Copy.ends_with("_"))
            continue;

          View = Copy;
          Alignment = Value;
          break;
        }
      }

      if (Alignment == Keyword::AlignSpec::NONE) {
        Check->diag(
            MacroNameTok.getLocation(),
            "missing or invalid alignment specification in keyword '%0'")
            << Name;
      }

      break;
    case Keyword::SectionType::CODE:
    default:
      break;
    }

    Keyword Keyword(MacroNameTok.getLocation(), Kind, Section, Policy,
                    Alignment, Name, Prefix.size(), Suffix.size());

    Check->add(std::move(Keyword));
  }

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {
    llvm::SmallString<256> Path;

    (void)IncludeTok;
    (void)FileName;
    (void)IsAngled;
    (void)FilenameRange;
    (void)File;
    (void)SuggestedModule;
    (void)ModuleImported;
    (void)FileType;

    // Return if this is one of clangd's replayed inclusion which are already
    // covered by explicitly parsing the preamble.
    if (Check->applyClangdAdjustments() && !RelativePath.ends_with(FileName))
      return;

    llvm::sys::path::append(Path, SearchPath, RelativePath);

    if (Check->isMemMapFile(Path)) {
      Check->add(Inclusion(HashLoc, Path));
      return;
    }

    // This callback provides a source location which in contrast to
    // 'LexedFileChanged' satisfies without further modifications the ordering
    // constraints on the memory mapping items used for this check.
    Check->add(FileBreak(HashLoc));
  }

  void LexedFileChanged(FileID FID, LexedFileChangeReason Reason,
                        SrcMgr::CharacteristicKind FileType, FileID PrevFID,
                        SourceLocation Loc) override {
    (void)FID;
    (void)FileType;
    (void)Loc;

    // Exclude entering and exiting memory mapping header files
    if (Reason != LexedFileChangeReason::ExitFile)
      return;

    auto &SM = Check->getSourceManager();
    const auto *FileEntry = SM.getFileEntryForID(PrevFID);
    if (!FileEntry)
      return;

    auto Path = FileEntry->tryGetRealPathName();

    if (Check->isMemMapFile(Path))
      return;

    auto EndOfFileLoc = SM.getLocForEndOfFile(PrevFID);
    Check->add(FileBreak(EndOfFileLoc));
  }

private:
  void checkAlignmentSpec(llvm::StringRef Affix) {}
  MemoryMappingCheck *Check;
};

//
// AUTOSAR Classic Platform R22-11: Specification of Memory Mapping
// [SWS_MemMap_00003] Each AUTOSAR basic software module and software component
// shall wrap declaration and definition of code, variables and constants using
// the following mechanism:
//      1. Definition of start symbol for module memory section
//      2. Inclusion of the memory mapping header file
//      3. Declaration/definition of code, variables or constants belonging to
//      the specified section
//      4. Definition of stop symbol for module memory section
//      5. Inclusion of the memory mapping header file
//
class CheckDetail {
public:
  CheckDetail(MemoryMappingCheck *Check,
              ArrayRef<MemoryMappingCheck::MemMapItem> MemMapItems,
              ArrayRef<MemoryMappingCheck::DeclItem> DeclItems)
      : Check(Check), MmItemIt(std::cbegin(MemMapItems)),
        MmItemEnd(std::cend(MemMapItems)), DeclIt(std::cbegin(DeclItems)),
        DeclEnd(std::cend(DeclItems)), SymbolMap(DeclItems.size()),
        PrevKeyword(nullptr), PrevStartInc(nullptr),
        ActiveFunctionDecl(nullptr), ActiveState(State::READY) {}

  void operator()(const VarDeclProxy &Proxy) {
    const auto *Decl = Proxy.get();

    switch (ActiveState) {
    case State::KEYWORD_STOP:
      Check->diag(PrevKeyword->getLocation(),
                  "missing inclusion of memory mapping header");
      ActiveState = State::READY;
      LLVM_FALLTHROUGH;
    case State::READY:
      Check->diag(Proxy.getLocation(), "declaration not memory mapped");

      break;
    case State::KEYWORD_START:
      Check->diag(PrevKeyword->getLocation(),
                  "missing inclusion of memory mapping header");
      LLVM_FALLTHROUGH;
    case State::DECL_READY:
      ActiveState = State::DECL;

      LLVM_FALLTHROUGH;
    case State::DECL:
      break;
    default:
      break;
    }

    // Ensure the memory mapping construct is well-formed. and that
    // 'PrevKeyword' points to a valid object.
    if (ActiveState != State::DECL)
      return;

    checkKeywordConsistency(Decl);

    // Ensure that every variable is mapped to an appropriate memory section.
    switch (PrevKeyword->getSectionType()) {
    case Keyword::SectionType::CONST:
      if (!Decl->getType().isConstQualified()) {
        Check->diag(
            Proxy.getLocation(),
            "invalid mapping of non-const variable '%0' to 'CONST' section")
            << Decl->getName();
        Check->diag(PrevKeyword->getLocation(),
                    "section started with '%0' here", DiagnosticIDs::Note)
            << PrevKeyword->getName();
        break;
      }
      break;
    case Keyword::SectionType::CODE:
      Check->diag(Proxy.getLocation(),
                  "invalid mapping of variable '%0' to 'CODE' section")
          << Decl->getName();
      Check->diag(PrevKeyword->getLocation(), "section started with '%0' here",
                  DiagnosticIDs::Note)
          << PrevKeyword->getName();
      break;
    case Keyword::SectionType::VAR:
      if (Decl->getType().isConstQualified()) {
        Check->diag(Proxy.getLocation(),
                    "invalid mapping of const variable '%0' to 'VAR' section");
        Check->diag(PrevKeyword->getLocation(),
                    "section started with '%0' here", DiagnosticIDs::Note)
            << PrevKeyword->getName();
      }
      break;
    default:
      break;
    }

    // Ensure that every variable is initialized according to the
    // initialization policy of the used memory section.
    switch (Decl->isThisDeclarationADefinition()) {
    case VarDecl::DefinitionKind::Definition:
      break;
    case VarDecl::DefinitionKind::TentativeDefinition:
      if (Decl != Decl->getActingDefinition())
        return;

      break;
    case VarDecl::DefinitionKind::DeclarationOnly:
    default:
      return;
    }

    switch (PrevKeyword->getInitPolicy()) {
    case Keyword::InitPolicy::CLEARED:
      //
      // AUTOSAR Classic Platform R22-11: Specification of Memory Mapping
      // "CLEARED, used for not explicitly initialized variables.
      // [...]
      // POWER_ON_CLEARED, used for variables that are not explicitly
      // initialized (cleared) during normal start-up. [...]"
      //
      if (!Decl->hasInit())
        break;

      Check->diag(Decl->getInit()->getExprLoc(),
                  "invalid explicit initialization of '%0'")
          << Decl->getName();
      Check->diag(PrevKeyword->getLocation(),
                  "'%0' allows only implicit initializiations",
                  DiagnosticIDs::Note)
          << PrevKeyword->getName();
      break;
    case Keyword::InitPolicy::INIT:
      //
      // AUTOSAR Classic Platform R22-11: Specification of Memory Mapping
      // "INIT, used for initialized variables. This are typically explicitly
      // initialized variables, but it can be also used for not explicitly
      // initialized variables [...]."
      //
      if (Check->allowLegacyInitialization() || Decl->hasInit())
        break;

      Check->diag(Decl->getLocation(),
                  "missing explicit initialization of '%0'")
          << Decl->getName();
      Check->diag(PrevKeyword->getLocation(),
                  "'%0' requires explicit initializations", DiagnosticIDs::Note)
          << PrevKeyword->getName();
      break;
    default:
      break;
    }
  }

  void operator()(const FunctionDeclProxy &Proxy) {
    const auto *Decl = Proxy.get();

    if (Decl->isThisDeclarationADefinition()) {
      if (Proxy.isStartLoc())
        ActiveFunctionDecl = Decl;
      else
        ActiveFunctionDecl = nullptr;
    }

    switch (ActiveState) {
    case State::KEYWORD_STOP:
      Check->diag(PrevKeyword->getLocation(),
                  "missing inclusion of memory mapping header");
      ActiveState = State::READY;
      LLVM_FALLTHROUGH;
    case State::READY:
      if (Proxy.isStartLoc()) {
        Check->diag(Proxy.getLocation(), "declaration not memory mapped");
      } else {
        Check->diag(Proxy.getLocation(),
                    "end of function body not memory mapped");
      }

      break;
    case State::KEYWORD_START:
      Check->diag(PrevKeyword->getLocation(),
                  "missing inclusion of memory mapping header");
      LLVM_FALLTHROUGH;
    case State::DECL_READY:
      ActiveState = State::DECL;

      LLVM_FALLTHROUGH;
    case State::DECL:
      break;
    default:
      break;
    }

    if (ActiveState != State::DECL)
      return;

    checkKeywordConsistency(Decl);

    // Ensure every function is mapped to an appropriate section.
    StringRef SectionTypeName;

    switch (PrevKeyword->getSectionType()) {
    case Keyword::SectionType::CODE:
    default:
      return;
    case Keyword::SectionType::VAR:
      SectionTypeName = "VAR";
      break;
    case Keyword::SectionType::CONST:
      SectionTypeName = "CONST";
      break;
    }

    Check->diag(Decl->getLocation(),
                "invalid mapping of function '%0' to '%1' section")
        << Decl->getName() << SectionTypeName;
    Check->diag(PrevKeyword->getLocation(), "section started with '%0' here",
                DiagnosticIDs::Note)
        << PrevKeyword->getName();
  }

  void operator()(const FileBreak &Break) {
    switch (ActiveState) {
    case State::READY:
      break;
    case State::KEYWORD_START:
      Check->diag(PrevKeyword->getLocation(),
                  "missing inclusion of memory mapping header");
      break;
    case State::DECL_READY:
    case State::DECL:
      Check->diag(Break.getLocation(),
                  "memory mapping section started by '%0' not stopped")
          << PrevKeyword->getName();
      break;
    case State::KEYWORD_STOP:
      Check->diag(PrevKeyword->getLocation(),
                  "missing inclusion of memory mapping header");
      break;
    default:
      break;
    }

    // Reset to a clean state at the start of every included file
    ActiveState = State::READY;
    PrevStartInc = nullptr;
    PrevKeyword = nullptr;
  }

  void operator()(const Keyword &Keyword) {
    switch (ActiveState) {
    case State::READY:
      if (Keyword.isStop()) {
        Check->diag(Keyword.getLocation(), "no memory mapping section started");

        ActiveState = State::KEYWORD_STOP;
        break;
      }

      checkRedundancy(Keyword);

      ActiveState = State::KEYWORD_START;
      break;
    case State::KEYWORD_START:
      Check->diag(Keyword.getLocation(),
                  "missing inclusion of memory mapping header");

      // Recovery strategy
      if (Keyword.isStop())
        ActiveState = State::KEYWORD_STOP;

      PrevStartInc = nullptr;

      break;
    case State::DECL_READY:
      if (!Check->allowEmptySections() && Keyword.isStop()) {
        Check->diag(PrevKeyword->getLocation(),
                    "unallowed empty memory mapping section");
        Check->diag(Keyword.getLocation(), "empty memory section ends here",
                    DiagnosticIDs::Note);
      }

      LLVM_FALLTHROUGH;
    case State::DECL:
      if (Keyword.isStart()) {
        Check->diag(PrevKeyword->getLocation(),
                    "memory mapping section started by '%0' not stopped")
            << PrevKeyword->getName();
        Check->diag(Keyword.getLocation(), "next section starts here",
                    DiagnosticIDs::Note);

        ActiveState = State::KEYWORD_START;
        break;
      }

      if (PrevKeyword->getPrefixAndSuffix() != Keyword.getPrefixAndSuffix()) {
        Check->diag(Keyword.getLocation(), "previous '%0' does not match '%1'")
            << PrevKeyword->getName() << Keyword.getName();
      }

      ActiveState = State::KEYWORD_STOP;
      break;
    case State::KEYWORD_STOP:
      Check->diag(PrevKeyword->getLocation(),
                  "missing inclusion of memory mapping header");

      if (Keyword.isStart()) {
        ActiveState = State::KEYWORD_START;
        break;
      }

      // We've encountered a stop symbol so we are still expecting an
      // inclusion of a memory mapping header for the next item.
      break;
    default:
      break;
    }

    // Always keep track of the previous keyword so we can always safely
    // reference its location in diagnostic messages.
    PrevKeyword = &Keyword;
  }

  void operator()(const Inclusion &Inc) {
    switch (ActiveState) {
    case State::READY:
      // We do not change the 'ActiveState' as we do not have a valid value
      // to which 'PrevKeyword' could point to which is an invariant within
      // the current implementation of the other states.
      Check->diag(Inc.getLocation(),
                  "missing keyword at start of memory mapping section");
      break;
    case State::KEYWORD_START:
      checkInvalidInclusion(Inc);

      PrevStartInc = &Inc;
      ActiveState = State::DECL_READY;
      break;
    case State::DECL_READY:
    case State::DECL:
      Check->diag(Inc.getLocation(),
                  "missing keyword at end of memory mapping section");

      ActiveState = State::READY;
      break;
    case State::KEYWORD_STOP:
      checkInvalidInclusion(Inc);

      if (PrevStartInc && Inc.getName() != PrevStartInc->getName()) {
        Check->diag(Inc.getLocation(),
                    "file name mismatch with previous inclusion");
      }

      ActiveState = State::READY;
      break;
    default:
      break;
    }
  }

  void run() {
    auto LocExtractor = [](auto &&Item) { return Item.getLocation(); };

    auto &SM = Check->getSourceManager();

    while (MmItemIt < MmItemEnd && DeclIt < DeclEnd) {
      auto MmItemLoc = std::visit(LocExtractor, *MmItemIt);
      auto DeclLoc = std::visit(LocExtractor, *DeclIt);

      if (SM.isBeforeInTranslationUnit(MmItemLoc, DeclLoc))
        std::visit(*this, *MmItemIt++);
      else
        std::visit(*this, *DeclIt++);
    }

    while (MmItemIt < MmItemEnd)
      std::visit(*this, *MmItemIt++);

    while (DeclIt < DeclEnd)
      std::visit(*this, *DeclIt++);

    // Perform final check to ensure correctness of the last memory section.
    auto FID = SM.getMainFileID();
    auto Loc = SM.getLocForEndOfFile(FID);
    MemoryMappingCheck::MemMapItem Item = FileBreak(Loc);

    std::visit(*this, Item);
  }

private:
  void checkKeywordConsistency(const DeclaratorDecl *Decl) {
    // Check if the same memory mapping macro is used for all declarations
    // referring to one specific identifier.
    const auto *FirstDecl = Decl->getCanonicalDecl();

    auto [It, Ok] = SymbolMap.insert({FirstDecl, PrevKeyword});
    if (Ok || It == SymbolMap.end())
      return;

    const auto *Keyword = It->second;
    if (Keyword->getName() == PrevKeyword->getName())
      return;

    Check->diag(Decl->getLocation(), "entity mapped with different keywords: "
                                     "'%0' and '%1'")
        << Keyword->getName() << PrevKeyword->getName();
  }

  void checkRedundancy(const Keyword &Keyword) {
    if (Check->allowRedundantSectionBreaks())
      return;

    if (!PrevKeyword || !PrevKeyword->isStop())
      return;

    if (PrevKeyword->getPrefixAndSuffix() != Keyword.getPrefixAndSuffix())
      return;

    Check->diag(PrevKeyword->getLocation(),
                "unallowed redundant break of memory mapping section");
    Check->diag(Keyword.getLocation(),
                "redundant break of memory mapping section ends here",
                DiagnosticIDs::Note);
  }

  void checkInvalidInclusion(const Inclusion &Inc) {
    if (ActiveFunctionDecl && !Check->allowStaticLocalVariables()) {
      Check->diag(Inc.getLocation(), "invalid inclusion of memory mapping "
                                     "header inside the body of a function");
    }
  }

  enum class State : uint8_t {
    READY,
    KEYWORD_START,
    DECL_READY,
    DECL,
    KEYWORD_STOP
  };

  MemoryMappingCheck *Check;
  std::vector<MemoryMappingCheck::MemMapItem>::const_iterator MmItemIt;
  std::vector<MemoryMappingCheck::MemMapItem>::const_iterator MmItemEnd;
  std::vector<MemoryMappingCheck::DeclItem>::const_iterator DeclIt;
  std::vector<MemoryMappingCheck::DeclItem>::const_iterator DeclEnd;
  llvm::DenseMap<const Decl *, const Keyword *> SymbolMap;

  const Keyword *PrevKeyword;
  const Inclusion *PrevStartInc;
  const FunctionDecl *ActiveFunctionDecl;
  State ActiveState;
};

} // namespace

MemoryMappingCheck::MemoryMappingCheck(StringRef Name,
                                       ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), MemMapItems(), DeclItems(),
      PrevFunctionDecl(nullptr), SrcM(nullptr), MemMapInclusions(),
      KeywordKinds(), SectionTypes(), InitPolicies(), AlignSpecs(),
      AllowEmptySections(false), AllowLegacyInitializations(false),
      AllowRedundantSectionBreaks(false), AllowStaticLocalVariables(false),
      ApplyClangdAdjustments(false), StrictAlignmentSpecifications(false) {

  MemMapItems.reserve(64);
  DeclItems.reserve(64);

  initMemMapInclusions();

  auto Compare = [](auto &&LHS, auto &&RHS) {
    return LHS.first.size() >= RHS.first.size();
  };

  splitOptionValue("KeywordStartAffix", DefaultKeywordSectionStart,
                   KeywordKinds, Keyword::Kind::START);
  splitOptionValue("KeywordStopAffix", DefaultKeywordSectionStop, KeywordKinds,
                   Keyword::Kind::STOP);
  llvm::sort(KeywordKinds, Compare);

  splitOptionValue("SectionCode", DefaultSectionCode, SectionTypes,
                   Keyword::SectionType::CODE);
  splitOptionValue("SectionConst", DefaultSectionConst, SectionTypes,
                   Keyword::SectionType::CONST);
  splitOptionValue("SectionVar", DefaultSectionVar, SectionTypes,
                   Keyword::SectionType::VAR);
  llvm::sort(SectionTypes, Compare);

  splitOptionValue("PolicyCleared", DefaultPolicyCleared, InitPolicies,
                   Keyword::InitPolicy::CLEARED);
  splitOptionValue("PolicyInit", DefaultPolicyInit, InitPolicies,
                   Keyword::InitPolicy::INIT);
  llvm::sort(InitPolicies, Compare);

  splitOptionValue("AlignBoolean", DefaultAlignBoolean, AlignSpecs,
                   Keyword::AlignSpec::BOOLEAN);
  splitOptionValue("Align8", DefaultAlign8, AlignSpecs, Keyword::AlignSpec::_8);
  splitOptionValue("Align16", DefaultAlign16, AlignSpecs,
                   Keyword::AlignSpec::_16);
  splitOptionValue("Align32", DefaultAlign32, AlignSpecs,
                   Keyword::AlignSpec::_32);
  splitOptionValue("Align64", DefaultAlign64, AlignSpecs,
                   Keyword::AlignSpec::_64);
  splitOptionValue("AlignPointer", DefaultAlignPointer, AlignSpecs,
                   Keyword::AlignSpec::PTR);
  splitOptionValue("AlignUnspecified", DefaultAlignUnspecified, AlignSpecs,
                   Keyword::AlignSpec::UNSPECIFIED);
  llvm::sort(AlignSpecs, Compare);

  AllowEmptySections = Options.get("AllowEmptySections", false);
  AllowLegacyInitializations = Options.get("AllowLegacyInitializations", false);
  AllowRedundantSectionBreaks =
      Options.get("AllowRedundantSectionBreaks", false);

  AllowStaticLocalVariables = Options.get("AllowStaticLocalVariables", false);
  ApplyClangdAdjustments = Options.get("ApplyClangdAdjustments", true);
  StrictAlignmentSpecifications =
      Options.get("StrictAlignmentSpecifications", false);
}

void MemoryMappingCheck::registerPPCallbacks(const SourceManager &SM,
                                             Preprocessor *PP,
                                             Preprocessor *ModuleExpanderPP) {

  (void)ModuleExpanderPP;

  SrcM = &SM;

  PP->addPPCallbacks(std::make_unique<CallbackManager>(this));

  if (!ApplyClangdAdjustments)
    return;

  const auto &PPOpts = PP->getPreprocessorOpts();
  auto PreambleSize = PPOpts.PrecompiledPreambleBytes.first;
  if (PreambleSize == 0)
    return;

  class PreprocessorAction : public PreprocessOnlyAction {
  public:
    explicit PreprocessorAction(MemoryMappingCheck *Check,
                                const Preprocessor *PP)
        : Check(Check), SM(nullptr), PP(PP) {}

  protected:
    bool BeginSourceFileAction(CompilerInstance &CI) override {
      SM = Check->SrcM;
      Check->SrcM = &CI.getSourceManager();

      // As this action is used within a clangd process, ensure we do not
      // accidentally leak memory.
      CI.getFrontendOpts().DisableFree = false;

      auto Manager = std::make_unique<CallbackManager>(Check);

      auto &NewPP = CI.getPreprocessor();

      NewPP.addPPCallbacks(std::move(Manager));

      // Use same header search information as the original frontend action.
      // We just copy the search paths as this seems to be sufficent for the
      // preprocessor to find the included files.
      auto &NewHSInfo = NewPP.getHeaderSearchInfo();
      auto &HSInfo = PP->getHeaderSearchInfo();

      std::vector<DirectoryLookup> Lookups(HSInfo.search_dir_begin(),
                                           HSInfo.search_dir_end());
      llvm::DenseMap<unsigned, unsigned> Map;

      auto AngledDirIdx = HSInfo.searchDirIdx(*HSInfo.angled_dir_begin());
      auto SysDirIdx = HSInfo.searchDirIdx(*HSInfo.system_dir_begin());

      // As we are skipping the user entries from the header search options
      // there is no need to map search directories to user entries.
      NewHSInfo.SetSearchPaths(Lookups, AngledDirIdx, SysDirIdx, Map);

      auto &NewHSOpts = NewPP.getHeaderSearchInfo().getHeaderSearchOpts();
      auto &HSOpts = PP->getHeaderSearchInfo().getHeaderSearchOpts();

      NewHSOpts.ResourceDir = HSOpts.ResourceDir;

      auto &NewPPOpts = NewPP.getPreprocessorOpts();
      auto &PPOpts = PP->getPreprocessorOpts();

      NewPPOpts.Macros = PPOpts.Macros;

      // Don't parse files included in the main file.
      NewPPOpts.SingleFileParseMode = true;

      return true;
    }

    void EndSourceFileAction() override { Check->SrcM = SM; }

  private:
    MemoryMappingCheck *Check;
    const SourceManager *SM;
    const Preprocessor *PP;
  };

  auto Buffer = SrcM->getBufferDataOrNone(SrcM->getMainFileID());
  if (!Buffer) {
    configurationDiag("failed to load clangd preamble", DiagnosticIDs::Error);
    return;
  }

  auto Preamble = Buffer.value().take_front(PreambleSize);

  auto &Files = SM.getFileManager();
  auto WorkingDir = Files.getVirtualFileSystem().getCurrentWorkingDirectory();
  if (!WorkingDir) {
    configurationDiag("failed to retrieve compilation directory: %0",
                      DiagnosticIDs::Error)
        << WorkingDir.getError().message();
    return;
  }

  IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem> VirtualFS =
      new llvm::vfs::OverlayFileSystem(Files.getVirtualFileSystemPtr());
  IntrusiveRefCntPtr<llvm::vfs::InMemoryFileSystem> InMemoryFileSystem =
      new llvm::vfs::InMemoryFileSystem();

  if (auto Error = VirtualFS->setCurrentWorkingDirectory(WorkingDir.get())) {
    configurationDiag("failed to set compilation directory: %0",
                      DiagnosticIDs::Error)
        << Error.message();
    return;
  }

  auto BufferCopy = llvm::MemoryBuffer::getMemBufferCopy(Preamble);

  auto MainFile = getCurrentMainFile();
  InMemoryFileSystem->addFile(MainFile, 0, std::move(BufferCopy));

  VirtualFS->pushOverlay(InMemoryFileSystem);

  auto Action = std::make_unique<PreprocessorAction>(this, PP);
  std::vector<std::string> Args;

  auto Ok = tooling::runToolOnCodeWithArgs(std::move(Action), Preamble,
                                           VirtualFS, Args, MainFile);
  if (!Ok) {
    configurationDiag("encountered errors while parsing clangd's preamble",
                      DiagnosticIDs::Error);
    return;
  }
}

void MemoryMappingCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(traverse(TK_IgnoreUnlessSpelledInSource, functionDecl())
                         .bind(MatchIDFunctionDecl),
                     this);

  Finder->addMatcher(
      traverse(TK_IgnoreUnlessSpelledInSource, varDecl(hasGlobalStorage()))
          .bind(MatchIDVarDecl),
      this);
}

void MemoryMappingCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Decl =
          Result.Nodes.getNodeAs<FunctionDecl>(MatchIDFunctionDecl)) {
    if (PrevFunctionDecl) {
      auto Loc = PrevFunctionDecl->getBody()->getEndLoc();

      DeclItems.push_back(FunctionDeclProxy(Loc, PrevFunctionDecl));
      PrevFunctionDecl = nullptr;
    }

    if (Decl->doesThisDeclarationHaveABody())
      PrevFunctionDecl = Decl;

    DeclItems.push_back(FunctionDeclProxy(Decl));

    return;
  }

  if (const auto *Decl = Result.Nodes.getNodeAs<VarDecl>(MatchIDVarDecl)) {
    if (!Decl->isStaticLocal() && PrevFunctionDecl) {
      auto Loc = PrevFunctionDecl->getBody()->getEndLoc();
      DeclItems.push_back(FunctionDeclProxy(Loc, PrevFunctionDecl));

      PrevFunctionDecl = nullptr;
    }

    DeclItems.push_back(VarDeclProxy(Decl));
    return;
  }
}

void MemoryMappingCheck::onEndOfTranslationUnit() {
  // There might be a left-over function from the AST matching step.
  if (PrevFunctionDecl) {
    auto Loc = PrevFunctionDecl->getBody()->getEndLoc();

    DeclItems.push_back(FunctionDeclProxy(Loc, PrevFunctionDecl));
  }

  CheckDetail(this, MemMapItems, DeclItems).run();
}

void MemoryMappingCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  OPTION_SET(Options, Opts, "MemMapInclusion", DefaultMemMapInclusion);

  OPTION_SET(Options, Opts, "KeywordStartAffix", DefaultKeywordSectionStart);
  OPTION_SET(Options, Opts, "KeywordStopAffix", DefaultKeywordSectionStop);

  OPTION_SET(Options, Opts, "SectionCode", DefaultSectionCode);
  OPTION_SET(Options, Opts, "SectionConst", DefaultSectionConst);
  OPTION_SET(Options, Opts, "SectionVar", DefaultSectionVar);

  OPTION_SET(Options, Opts, "PolicyCleared", DefaultPolicyCleared);
  OPTION_SET(Options, Opts, "PolicyInit", DefaultPolicyInit);

  OPTION_SET(Options, Opts, "AlignBoolean", DefaultAlignBoolean);
  OPTION_SET(Options, Opts, "Align8", DefaultAlign8);
  OPTION_SET(Options, Opts, "Align16", DefaultAlign16);
  OPTION_SET(Options, Opts, "Align32", DefaultAlign32);
  OPTION_SET(Options, Opts, "Align64", DefaultAlign64);
  OPTION_SET(Options, Opts, "AlignPointer", DefaultAlignPointer);
  OPTION_SET(Options, Opts, "AlignUnspecified", DefaultAlignUnspecified);

  OPTION_SET_VARIABLE(Options, Opts, AllowEmptySections);
  OPTION_SET_VARIABLE(Options, Opts, AllowLegacyInitializations);
  OPTION_SET_VARIABLE(Options, Opts, AllowStaticLocalVariables);
  OPTION_SET_VARIABLE(Options, Opts, AllowRedundantSectionBreaks);
  OPTION_SET_VARIABLE(Options, Opts, ApplyClangdAdjustments);
  OPTION_SET_VARIABLE(Options, Opts, StrictAlignmentSpecifications);
}

void MemoryMappingCheck::initMemMapInclusions() {
  StringRef Key = "MemMapInclusion";

  auto Value = Options.get(Key, DefaultMemMapInclusion);

  MemMapInclusions.reserve(Value.count(';') + 1);

  while (!Value.empty()) {
    auto [Head, Tail] = Value.split(';');

    Value = Tail;
    Head = Head.trim();

    if (Head.empty()) {
      configurationDiag("invalid empty value in option '%0'") << Key;
      continue;
    }

    auto ExpectedGlob = llvm::GlobPattern::create(Head);
    if (!ExpectedGlob) {
      configurationDiag("invalid glob pattern in option '%0': %1")
          << Key << ExpectedGlob.takeError();
      continue;
    }

    MemMapInclusions.push_back(std::move(ExpectedGlob.get()));
  }
}

} // namespace clang::tidy::autosar
