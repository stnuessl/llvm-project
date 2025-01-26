//===--- InterfaceConnectedCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InterfaceConnectedCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::autosar {

namespace {

class CallbackManager : public PPCallbacks {
public:
  CallbackManager(InterfaceConnectedCheck *Check) : Check(Check) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    const auto *Info = MD->getMacroInfo();

    if (Info->isFunctionLike()) {
      auto Name = MacroNameTok.getIdentifierInfo()->getName();

      if (!Check->isMacroInterface(Name))
        return;

      /* Check if this macro is itself an unconnected macro interface */
      for (const auto &Token : Info->tokens()) {
        const auto *IdInfo = Token.getIdentifierInfo();
        if (!IdInfo)
          continue;

        if (IdInfo->getName() != "RTE_E_UNCONNECTED")
          continue;

        Check->diag(Info->getDefinitionLoc(),
                    "unconnected macro interface '%0'")
            << Name;

        break;
      }

      return;
    }
  }

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    auto Name = MacroNameTok.getIdentifierInfo()->getName();
    const auto *Info = MD.getMacroInfo();

    if (Info->isFunctionLike()) {
      if (!Check->isMacroInterface(Name))
        return;

      if (!Check->addMacroInterface(Range.getBegin(), Name)) {
        Check->diag(Range.getBegin(), "internal: failed to process expansion "
                                      "of function-like macro '%0'")
            << Name;

        return;
      }

      return;
    }

    if (Name != "RTE_E_UNCONNECTED")
      return;

    if (!Check->addUnconnectedMacroLocation(Range.getBegin())) {
      Check->diag(
          Range.getBegin(),
          "internal: failed to process expansion of 'RTE_E_UNCONNECTED'");

      return;
    }
  }

private:
  InterfaceConnectedCheck *Check;
};

AST_MATCHER_P(FunctionDecl, matchesRegex, const llvm::Regex *, Regex) {
  return Regex->match((&Node)->getName());
}

AST_MATCHER(QualType, stdReturnType) {
  const auto *Type = Node->getAs<TypedefType>();
  if (!Type)
    return false;

  return Type->getDecl()->getName() == "Std_ReturnType";
}

AST_MATCHER_P(Expr, hasUnconnectedMacroLocation,
              const InterfaceConnectedCheck *, Check) {
  return Check->isUnconnectedMacroLocation((&Node)->getExprLoc());
}

AST_MATCHER_P(Expr, isUnconnectedInterface, const InterfaceConnectedCheck *,
              Check) {

  return Check->isMacroInterface((&Node)->getExprLoc()) &&
         Check->isUnconnectedMacroLocation(
             (&Node)->IgnoreParenCasts()->getExprLoc());
}

} // namespace

void InterfaceConnectedCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {

  (void)ModuleExpanderPP;

  SrcM = &SM;

  PP->addPPCallbacks(std::make_unique<CallbackManager>(this));
}

void InterfaceConnectedCheck::registerMatchers(MatchFinder *Finder) {
  /* clang-format off */

  Finder->addMatcher(
    functionDecl(
      isDefinition(),
      returns(stdReturnType()),
      matchesRegex(&FunctionRegex),
      hasBody(
        hasDescendant(returnStmt(hasReturnValue(ignoringParenCasts(anyOf(
          hasUnconnectedMacroLocation(this),
          declRefExpr(to(varDecl(
            hasLocalStorage(),
            hasType(stdReturnType()),
            hasInitializer(ignoringParenCasts(
              hasUnconnectedMacroLocation(this)
            ))
          )))
        )))))
      )
    )
    .bind("Func"),
    this
  );

  /* clang-format on */

  /*
   * Checking the AST for expressions making use of unconnected interfaces
   * ensures that the interfaces are actually used. This wouldn't be possible
   * by just using the preprocesser.
   */
  Finder->addMatcher(expr(isUnconnectedInterface(this)).bind("Expr"), this);
}

void InterfaceConnectedCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Match = Result.Nodes.getNodeAs<FunctionDecl>("Func")) {
    diag(Match->getLocation(), "unconnected function interface '%0'")
        << Match->getName();
    return;
  }

  if (const auto *Match = Result.Nodes.getNodeAs<Expr>("Expr")) {

    auto Loc = Match->getExprLoc();
    diag(Loc, "usage of unconnected macro interface '%0'")
        << getMacroInterfaceName(Loc);

    return;
  }
}

} // namespace clang::tidy::autosar
