//===--- Rule17p7Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P7CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P7CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(CallExpr, hasUnusedReturnValue) {
  using namespace clang::ast_matchers;

  // clang-format off
  return callExpr(
    unless(hasType(voidType())),
    hasParent(stmt(anyOf(
      compoundStmt(),
      mapAnyOf(
        doStmt,
        forStmt,
        ifStmt,
        whileStmt,
        switchStmt
      ).with(
        unless(hasCondition(equalsNode(&Node)))
      ),
      switchCase(),
      binaryOperator(
        hasOperatorName(","),
        hasLHS(equalsNode(&Node))
      )
    )))
  ).matches(Node, Finder, Builder);
  // clang-format on
}

} // namespace

/// Detects issues in accordance with MISRA C rule 17.7:
/// "The value returned by a function having non-void return type shall be
/// used".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-17p7.html
class Rule17p7Check : public MisracBaseCheck {
public:
  Rule17p7Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(callExpr(hasUnusedReturnValue()).bind("expr"), this);
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Expr = Result.Nodes.getNodeAs<CallExpr>("expr");

    diag(Expr->getExprLoc(), "unused return value")
        << Expr->getSourceRange()
        << FixItHint::CreateInsertion(Expr->getExprLoc(), "(void) ");
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P7CHECK_H
