//===--- MissingCompoundCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingCompoundCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Transformer/SourceCode.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

namespace {

AST_MATCHER(SwitchStmt, hasCompoundStmt) {
  return (&Node)->getBody()->getStmtClass() == Stmt::CompoundStmtClass;
}

} // namespace

void MissingCompoundCheck::registerMatchers(MatchFinder *Finder) {

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

void MissingCompoundCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Stmt = Result.Nodes.getNodeAs<DoStmt>("do")) {
    diag(Stmt->getBody(), Result.Context,
         "the body of a 'do while loop' must be a compound statement");

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<ForStmt>("for")) {
    diag(Stmt->getBody(), Result.Context,
         "the body of a 'for loop' must be a compound statement");

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<IfStmt>("if")) {
    const auto *ThenStmt = Stmt->getThen();
    if (!isa<CompoundStmt>(ThenStmt)) {
      diag(ThenStmt, Result.Context,
           "the then branch of an 'if condition' must be a compound statement");
    }

    const auto *ElseStmt = Stmt->getElse();
    if (ElseStmt && !isa<CompoundStmt>(ElseStmt)) {
      diag(ElseStmt, Result.Context,
           "the else branch of an 'if condition' must be a compound statement");
    }

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
    diag(Stmt->getBody(), Result.Context,
         "the body of a 'switch' must be a compound statement");

    return;
  }

  if (const auto *Stmt = Result.Nodes.getNodeAs<WhileStmt>("while")) {
    diag(Stmt->getBody(), Result.Context,
         "the body of a 'while loop' must be a compound statement");

    return;
  }
}

DiagnosticBuilder MissingCompoundCheck::diag(const Stmt *Stmt,
                                             ASTContext *Context,
                                             StringRef Message) {
  switch (Stmt->getStmtClass()) {
  case Stmt::IfStmtClass:
  case Stmt::ForStmtClass:
  case Stmt::DoStmtClass:
  case Stmt::WhileStmtClass:
  case Stmt::SwitchStmtClass:
    return ClangTidyCheck::diag(Stmt->getBeginLoc(), Message);
  default:
    break;
  }

  auto Range = tooling::getExtendedRange(*Stmt, tok::TokenKind::semi, *Context);

  return ClangTidyCheck::diag(Range.getBegin(), Message)
         << FixItHint::CreateInsertion(Range.getBegin(), "{ ")
         << FixItHint::CreateReplacement(Range.getEnd(), "; }");
}

} // namespace clang::tidy::embedded
