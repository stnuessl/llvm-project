//===--- Rule9p3Check.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE9P3CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE9P3CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER_P(InitListExpr, isIdiomaticZeroInitializer, LangOptions, LangOpts) {
  const auto *InitListExpr = &Node;

  if (InitListExpr->isSyntacticForm())
    return InitListExpr->isIdiomaticZeroInitializer(LangOpts);

  if (const auto *Expr = InitListExpr->getSyntacticForm())
    return Expr->isIdiomaticZeroInitializer(LangOpts);

  return false;
}

AST_MATCHER(InitListExpr, hasArrayFiller) { return (&Node)->hasArrayFiller(); }

} // namespace

/// Detects issues in accordance with MISRA C rule 9.3:
/// "Arrays shall not be partially initialized".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-9p3.html
class Rule9p3Check : public MisracBaseCheck {
public:
  Rule9p3Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    /* clang-format off */
    Finder->addMatcher(
      initListExpr(
        hasArrayFiller(),
        unless(isIdiomaticZeroInitializer(getLangOpts()))
      ).bind("expr"),
      this);
    /* clang-format on */
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Expr = Result.Nodes.getNodeAs<InitListExpr>("expr");
    const auto *Type = dyn_cast<ConstantArrayType>(Expr->getType());

    diag(Expr->getExprLoc(), "partial array initialization")
        << Expr->getSourceRange();

    if (Type) {
      diag(Expr->getExprLoc(), "%0 out of %1 elements initialized",
           DiagnosticIDs::Note)
          << Expr->getNumInits() << Type->getLimitedSize();
    }
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE9P3CHECK_H
