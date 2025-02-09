//===--- MacroInterfaceCheck.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MacroInterfaceCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::autosar {

namespace {

class CallbackManager : public PPCallbacks {
public:
  CallbackManager(MacroInterfaceCheck *Check) : Check(Check) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {

    auto Name = llvm::sys::path::filename(FileName);

    if (!Name.consume_front("Rte_"))
      return;

    Name.consume_front("Byps_");

    if (!Name.consume_back(".h"))
      return;

    if (Name == "Type")
      return;

    auto [Prefix, Suffix] = Name.split('_');

    if (!Suffix.empty())
      Check->setSwcName(Suffix);
    else
      Check->setSwcName(Prefix);
  }

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    const auto *IdentifierInfo = MacroNameTok.getIdentifierInfo();
    if (!IdentifierInfo)
      return;

    auto Name = IdentifierInfo->getName();

    if (!Name.starts_with("Rte_"))
      return;

    Check->addMacro(Name);
  }

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    const auto *IdentifierInfo = MacroNameTok.getIdentifierInfo();

    if (!IdentifierInfo)
      return;

    auto Name = IdentifierInfo->getName();

    if (!Name.starts_with("Rte_"))
      return;

    Check->addInterface(Range.getBegin(), Name);
  }

private:
  MacroInterfaceCheck *Check;
};

AST_MATCHER(FunctionDecl, isDefined) { return (&Node)->isDefined(); }

AST_MATCHER_P(NamedDecl, hasNamePrefix, StringRef, Prefix) {
  return (&Node)->getName().starts_with(Prefix);
}

AST_MATCHER_P(NamedDecl, hasNameAffix, StringRef, Affix) {
  return (&Node)->getName().contains(Affix);
}

AST_MATCHER_P(Decl, isMacroInterface, const MacroInterfaceCheck *, Check) {
  return Check->isMacroInterfaceLocation((&Node)->getLocation());
}

} // namespace

void MacroInterfaceCheck::registerPPCallbacks(const SourceManager &SM,
                                              Preprocessor *PP,
                                              Preprocessor *ModuleExpanderPP) {

  (void)ModuleExpanderPP;

  SrcM = &SM;

  PP->addPPCallbacks(std::make_unique<CallbackManager>(this));
}

void MacroInterfaceCheck::registerMatchers(MatchFinder *Finder) {
  // clang-format off

  Finder->addMatcher(
    functionDecl(
      hasNamePrefix("Rte_"),
      hasNameAffix(SwcName),
      unless(isDefined()),
      unless(isMacroInterface(this))
    ).bind("function"),
    this
  );

  // clang-format on
}

void MacroInterfaceCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
    StringRef Macro = "";
    auto [Prefix, Suffix] = Decl->getName().split(SwcName);

    for (const auto Name : Macros) {
      if (Name.starts_with(Prefix) && Name.ends_with(Suffix)) {
        Macro = Name;
        break;
      }
    }

    auto Diagnostic =
        diag(Decl->getLocation(),
             "interface '%0' should be used via its corresponding "
             "interface macro")
        << Decl->getName();

    if (!Macro.empty()) {
      Diagnostic << FixItHint::CreateReplacement(
          SourceRange(Decl->getLocation(), Decl->getLocation().getLocWithOffset(
                                               Decl->getName().size())),
          Macro);
    }

    return;
  }
}

} // namespace clang::tidy::autosar
