//===--- NoPartialArrayInitCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoPartialArrayInitCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

namespace {

AST_MATCHER_P(InitListExpr, isIdiomaticZeroInitializer, LangOptions, Opts) {
    return (&Node)->isIdiomaticZeroInitializer(Opts);
}

AST_MATCHER(InitListExpr, hasArrayFiller) {
    return (&Node)->hasArrayFiller();
}

} // namespace

void NoPartialArrayInitCheck::registerMatchers(MatchFinder *Finder) {
  /* clang-format off */

  Finder->addMatcher(
    initListExpr(
      hasArrayFiller(),
      unless(isIdiomaticZeroInitializer(getLangOpts()))
    ).bind("expr"),
    this);

  /* clang-format on */
}

void NoPartialArrayInitCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *Expr = Result.Nodes.getNodeAs<InitListExpr>("expr");

    diag(Expr->getExprLoc(), "disallowed partial array initialization")
        << FixItHint::CreateReplacement(Expr->getSourceRange(), "{0}");
}

} // namespace clang::tidy::embedded
