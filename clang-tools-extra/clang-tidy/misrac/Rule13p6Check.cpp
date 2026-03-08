//===--- Rule13p6Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule13p6Check.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(Expr, hasPotentialSideEffect) {
  return (&Node)->HasSideEffects(Finder->getASTContext(),
                                 /* IncludePossibleSideEffects */ true);
}

} // namespace

void Rule13p6Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  Finder->addMatcher(
    sizeOfExpr(
        has(ignoringParenImpCasts(hasPotentialSideEffect()))
    ).bind("sizeof"),
    this);
  // clang-format on
}

void Rule13p6Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
    const auto *Expr = Result.Nodes.getNodeAs<UnaryExprOrTypeTraitExpr>("sizeof");
    const auto *SubExpr = Expr->getArgumentExpr()->IgnoreParenImpCasts();

    diag(SubExpr->getExprLoc(), "'sizeof' argument has potential side-effect")
        << SubExpr->getSourceRange();
}

} // namespace clang::tidy::misrac
