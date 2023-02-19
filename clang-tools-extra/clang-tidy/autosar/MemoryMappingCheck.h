//===--- MemoryMappingCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_MEMORYMAPPINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_MEMORYMAPPINGCHECK_H

#include "../ClangTidyCheck.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/GlobPattern.h"

#include <variant>

namespace clang::tidy::autosar {

class FileBreak {
public:
  FileBreak() = default;
  FileBreak(SourceLocation Loc) : SLoc(Loc) {}

  SourceLocation getLocation() const { return SLoc; }

private:
  SourceLocation SLoc;
};

class Inclusion {
public:
  Inclusion() = default;
  Inclusion(SourceLocation Loc, StringRef FileName)
      : SLoc(Loc), Name(FileName) {}

  SourceLocation getLocation() const { return SLoc; }
  StringRef getName() const { return Name; }

private:
  SourceLocation SLoc;
  std::string Name;
};

/// A class representing a memory mapping keyword.
class Keyword {
public:
  enum class Kind : uint8_t { NONE, START, STOP };

  enum class SectionType : uint8_t {
    NONE,
    CODE,
    CONST,
    VAR,
  };

  enum class InitPolicy : uint8_t {
    NONE,
    CLEARED,
    INIT,
  };

  enum class AlignSpec : uint8_t {
    NONE,
    BOOLEAN,
    _8,
    _16,
    _32,
    _64,
    PTR,
    UNSPECIFIED,
  };

  Keyword() = default;
  Keyword(SourceLocation Loc, Kind Kind, SectionType Section, InitPolicy Policy,
          AlignSpec Alignment, StringRef Name, unsigned PrefixSize,
          unsigned SuffixSize)
      : Location(Loc), KeywordKind(Kind), Section(Section), Policy(Policy),
        Alignment(Alignment), Name(Name), PrefixSize(PrefixSize),
        SuffixSize(SuffixSize) {}

  std::pair<StringRef, StringRef> getPrefixAndSuffix() const {
    auto Value = getName();

    return {Value.take_front(PrefixSize), Value.take_back(SuffixSize)};
  }

  bool isStart() const { return KeywordKind == Kind::START; }
  bool isStop() const { return KeywordKind == Kind::STOP; }

  SourceLocation getLocation() const { return Location; }
  SectionType getSectionType() const { return Section; }
  InitPolicy getInitPolicy() const { return Policy; }
  AlignSpec getAlignment() const { return Alignment; }
  StringRef getName() const { return Name; }

private:
  SourceLocation Location;
  Kind KeywordKind;
  SectionType Section;
  InitPolicy Policy;
  AlignSpec Alignment;
  std::string Name;
  unsigned PrefixSize;
  unsigned SuffixSize;
};

// This class basically exists only for consistency reasons.
class VarDeclProxy {
public:
  explicit VarDeclProxy(const VarDecl *Decl)
      : VarDeclProxy(Decl->getLocation(), Decl) {}
  VarDeclProxy(SourceLocation Loc, const VarDecl *Decl)
      : Location(Loc), Decl(Decl) {}

  SourceLocation getLocation() const { return Location; }
  const VarDecl *get() const { return Decl; }

private:
  SourceLocation Location;
  const VarDecl *Decl;
};

// A function definition is a code block meaning it could potentially span
// multiple lines. To ensure correctness of the AUTOSAR memory mapping
// mechanism which heavily relies on the preprocessor, we need indicators
// at the start and at the end of the function definition.
class FunctionDeclProxy {
public:
  explicit FunctionDeclProxy(const FunctionDecl *Decl)
      : FunctionDeclProxy(Decl->getLocation(), Decl) {}
  FunctionDeclProxy(SourceLocation Loc, const FunctionDecl *Decl)
      : Location(Loc), Decl(Decl) {}

  SourceLocation getLocation() const { return Location; }
  bool isStartLoc() const { return Location == Decl->getLocation(); }
  bool isEndLoc() const { return Location != Decl->getLocation(); }
  const FunctionDecl *get() const { return Decl; }

private:
  SourceLocation Location;
  const FunctionDecl *Decl;
};

/// Detect memory mapping issues in AUTOSAR Classic Platform projects.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/autosar/memory-mapping.html
class MemoryMappingCheck : public ClangTidyCheck {
public:
  using MemMapItem = std::variant<FileBreak, Inclusion, Keyword>;
  using DeclItem = std::variant<VarDeclProxy, FunctionDeclProxy>;

  MemoryMappingCheck(StringRef Name, ClangTidyContext *Context);

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.C99 || LangOpts.C11 || LangOpts.C17;
  }

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void onEndOfTranslationUnit() override;

  std::optional<TraversalKind> getCheckTraversalKind() const override {
    return {TK_IgnoreUnlessSpelledInSource};
  }

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

  void add(MemMapItem Item) { MemMapItems.push_back(std::move(Item)); }

  const SourceManager &getSourceManager() const { return *SrcM; }

  bool isMemMapFile(StringRef Name) const {
    for (auto &Glob : MemMapInclusions) {
      if (Glob.match(Name))
        return true;
    }

    return false;
  }

  ArrayRef<std::pair<StringRef, Keyword::Kind>> getKeywordKinds() const {
    return KeywordKinds;
  }

  ArrayRef<std::pair<StringRef, Keyword::SectionType>> getSectionTypes() const {
    return SectionTypes;
  }

  ArrayRef<std::pair<StringRef, Keyword::InitPolicy>> getInitPolicies() const {
    return InitPolicies;
  }

  ArrayRef<std::pair<StringRef, Keyword::AlignSpec>> getAlignmentSpecs() const {
    return AlignSpecs;
  }

  bool allowEmptySections() const { return AllowEmptySections; }
  bool allowLegacyInitialization() const { return AllowLegacyInitializations; }
  bool allowStaticLocalVariables() const { return AllowStaticLocalVariables; }
  bool allowRedundantSectionBreaks() const {
    return AllowRedundantSectionBreaks;
  }
  bool applyClangdAdjustments() const { return ApplyClangdAdjustments; }
  bool strictAlignmentSpecifications() const {
    return StrictAlignmentSpecifications;
  }

private:
  template <typename T>
  void splitOptionValue(
      StringRef Key, StringRef Default,
      std::vector<std::pair<StringRef, T>> &Vec,
      typename std::enable_if<std::is_enum<T>::value, T>::type MappedValue) {

    auto Value = Options.get(Key, Default);

    Vec.reserve(Value.count(';') + 1);

    while (!Value.empty()) {
      auto [Head, Tail] = Value.split(';');

      Value = Tail;
      Head = Head.trim();

      if (Head.empty()) {
        configurationDiag("invalid empty value in option '%0'") << Key;
        continue;
      }

      Vec.push_back({Head, MappedValue});
    }
  }

  void initMemMapInclusions();

  std::vector<MemMapItem> MemMapItems;
  std::vector<DeclItem> DeclItems;
  const FunctionDecl *PrevFunctionDecl;
  const SourceManager *SrcM;

  // Options
  std::vector<llvm::GlobPattern> MemMapInclusions;
  std::vector<std::pair<StringRef, Keyword::Kind>> KeywordKinds;
  std::vector<std::pair<StringRef, Keyword::SectionType>> SectionTypes;
  std::vector<std::pair<StringRef, Keyword::InitPolicy>> InitPolicies;
  std::vector<std::pair<StringRef, Keyword::AlignSpec>> AlignSpecs;

  bool AllowEmptySections;
  bool AllowLegacyInitializations;
  bool AllowRedundantSectionBreaks;
  bool AllowStaticLocalVariables;
  bool ApplyClangdAdjustments;
  bool StrictAlignmentSpecifications;
};

} // namespace clang::tidy::autosar

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_MEMORYMAPPINGCHECK_H
