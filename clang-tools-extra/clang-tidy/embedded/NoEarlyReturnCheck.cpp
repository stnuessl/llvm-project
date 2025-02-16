//===--- NoEarlyReturnCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoEarlyReturnCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void NoEarlyReturnCheck::registerMatchers(MatchFinder *Finder) {
  /* clang-format off */

  Finder->addMatcher(
    returnStmt(
      hasAncestor(functionDecl().bind("decl"))
    ).bind("stmt"),
    this
  );

  /* clang-format on */
}

void NoEarlyReturnCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("decl");
  const auto *Stmt = Result.Nodes.getNodeAs<ReturnStmt>("stmt");

  if (Vec.empty() || Vec.back().first != Decl)
    Vec.push_back({Decl, {Stmt}});
  else
    Vec.back().second.push_back(Stmt);
}

void NoEarlyReturnCheck::onEndOfTranslationUnit() {
  auto Compare = [](const ReturnStmt *A, const ReturnStmt *B) {
    return A->getBeginLoc() < B->getBeginLoc();
  };

  for (auto &[Decl, Stmts] : Vec) {
    if (Stmts.empty())
      continue;

    std::sort(Stmts.begin(), Stmts.end(), Compare);

    const auto *LastReturnStmt = Stmts.back();

    const auto *Stmt = dyn_cast<CompoundStmt>(Decl->getBody());
    if (!Stmt->body_empty() && Stmt->body_back() != LastReturnStmt)
      diag(Stmt->getEndLoc(), "return statement missing at end of function");
    else
      Stmts.pop_back();

    for (const auto *ReturnStmt : Stmts)
      diag(ReturnStmt->getReturnLoc(), "disallowed early return statement");
  }
}

} // namespace clang::tidy::embedded
