//===--- Rule15p6Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule15p6Check.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(SwitchStmt, hasCompoundStmt) {
  return (&Node)->getBody()->getStmtClass() == Stmt::CompoundStmtClass;
}

} // namespace

void Rule15p6Check::addMatchers(MatchFinder *Finder) {
  /* clang-format off */
  Finder->addMatcher(
    doStmt(unless(hasBody(compoundStmt()))).bind("do"),
    this);

  Finder->addMatcher(
    forStmt(unless(hasBody(compoundStmt()))).bind("for"),
    this);

  Finder->addMatcher(
    ifStmt(
      unless(hasThen(compoundStmt())),
      unless(hasElse(compoundStmt()))
    ).bind("if"),
    this);

  Finder->addMatcher(
    switchStmt(unless(hasCompoundStmt())).bind("switch"),
    this);

  Finder->addMatcher(
    whileStmt(unless(hasBody(compoundStmt()))).bind("while"),
    this);
  /* clang-format on */
}

void Rule15p6Check::dispatch(const MatchFinder::MatchResult &Result) {
  if (const auto *Stmt = Result.Nodes.getNodeAs<DoStmt>("do")) {
    diag(Stmt->getBody());

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<ForStmt>("for")) {
    diag(Stmt->getBody());

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<IfStmt>("if")) {
    const auto *ThenStmt = Stmt->getThen();
    if (!isa<CompoundStmt>(ThenStmt))
      diag(ThenStmt);

    const auto *ElseStmt = Stmt->getElse();
    if (ElseStmt && !isa<CompoundStmt>(ElseStmt))
      diag(ElseStmt);

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
    diag(Stmt->getBody());

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<WhileStmt>("while")) {
    diag(Stmt->getBody());

    return;
  }
}

DiagnosticBuilder Rule15p6Check::diag(const Stmt *Stmt) {
  return MisracBaseCheck::diag(Stmt->getBeginLoc(),
                               "missing compound statement");
}

} // namespace clang::tidy::misrac
