//===--- Rule20p2Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule20p2Check.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::misrac {

namespace {

class IncludeChecker : public PPCallbacks {
public:
  explicit IncludeChecker(Rule20p2Check *Check) : Check(Check) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {

    (void)HashLoc;
    (void)IncludeTok;
    (void)IsAngled;
    (void)File;
    (void)SearchPath;
    (void)RelativePath;
    (void)SuggestedModule;
    (void)ModuleImported;
    (void)FileType;

    Check->dispatchInclusion(FileName, FilenameRange);
  }

private:
  Rule20p2Check *Check;
};

} // namespace

Rule20p2Check::Rule20p2Check(StringRef Name, ClangTidyContext *Context)
    : MisracBaseCheck(Name, Context), RegexPattern(Options.get("Regex", "")) {

  if (!RegexPattern.empty()) {
    std::string Error;
    Regex = llvm::Regex(RegexPattern);

    if (!Regex.isValid(Error)) {
      configurationDiag("invalid regex: %0", DiagnosticIDs::Error) << Error;
    }
  }
}

void Rule20p2Check::dispatchInclusion(StringRef FileName,
                                      CharSourceRange FilenameRange) {

  for (StringRef Item : {"'", "\"", "\\", "/*", "//"}) {
    size_t Start = 0;

    while (1) {
      auto Index = FileName.find(Item, Start);
      if (Index == StringRef::npos)
        break;

      // The file name range starts at the '<' or '"' character.
      auto Loc = FilenameRange.getBegin().getLocWithOffset(Index + 1);

      diag(Loc, Item);

      Start = Index + Item.size();
    }
  }

  // Assuming the regex is invalid, then an invalid regex diagnostic was
  // already emitted by this point.
  if (RegexPattern.empty() || !Regex.isValid())
    return;

  auto Buffer = SmallVector<StringRef, 8>();
  if (!Regex.match(FileName, &Buffer))
    return;

  for (auto Item : Buffer) {
    // The underlying pointers refer to the same object, so we can just
    // calculate the index instead of doing a substring search.
    auto Index = Item.begin() - FileName.begin();
    auto Loc = FilenameRange.getBegin().getLocWithOffset(Index + 1);

    diag(Loc, Item);
  }
}

void Rule20p2Check::addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                   Preprocessor *ModuleExpanderPP) {
  (void)SM;
  (void)ModuleExpanderPP;

  PP->addPPCallbacks(std::make_unique<IncludeChecker>(this));
}

void Rule20p2Check::writeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "Regex", RegexPattern);
}

DiagnosticBuilder Rule20p2Check::diag(SourceLocation Loc, StringRef Item) {
  // We cannot supply a SourceRange with 'getBegin()' == 'getEnd' as
  // otherwise the highlighted source range in the output of the diagnostic
  // message is wrong.

  if (Item.size() == 0)
    return MisracBaseCheck::diag(SourceLocation(), "");

  if (Item.size() == 1)
    return MisracBaseCheck::diag(Loc, "invalid character '%0'") << Item;

  return MisracBaseCheck::diag(Loc, "invalid character sequence '%0'")
         << SourceRange(Loc, Loc.getLocWithOffset(Item.size() - 1)) << Item;
}

} // namespace clang::tidy::misrac
