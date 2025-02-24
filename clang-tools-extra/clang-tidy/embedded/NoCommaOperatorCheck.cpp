//===--- NoCommaOperatorCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoCommaOperatorCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

namespace {

AST_MATCHER(BinaryOperator, isCommaOperator) {
  return (&Node)->getOpcode() == BO_Comma;
}

AST_MATCHER(BinaryOperator, any) {
  (void)Node;

  return true;
}

} // namespace

void NoCommaOperatorCheck::registerMatchers(MatchFinder *Finder) {
  // clang-format off

  auto RefinementMatcher = (StrictMode) ? any() : unless(hasParent(forStmt()));

  Finder->addMatcher(
    binaryOperator(
        isCommaOperator(),
        std::move(RefinementMatcher)
    ).bind("op"),
    this);

  // clang-format on
}

void NoCommaOperatorCheck::check(const MatchFinder::MatchResult &Result) {

  const auto *Op = Result.Nodes.getNodeAs<BinaryOperator>("op");
  auto Loc = Op->getExprLoc();

  if (!ReportAtMacroExpansion)
    Loc = Result.SourceManager->getSpellingLoc(Loc);

  diag(Loc, "disallowed use of the comma operator");
}

} // namespace clang::tidy::embedded
