//===--- Rule15p4Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule15p4Check.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace clang::tidy::misrac {

void Rule15p4Check::addMatchers(clang::ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  auto AnyIterationStmt =
      anyOf(cxxForRangeStmt(), doStmt(), forStmt(), whileStmt());

  // clang-format off
  Finder->addMatcher(
    stmt(
      AnyIterationStmt,
      unless(hasAncestor(stmt(AnyIterationStmt)))
    ).bind("stmt"),
    this);
  // clang-format on
}

void Rule15p4Check::onEndOfTranslationUnit() {
  for (auto &[IterationStmt, ExitStmtVec] : Map) {
    unsigned LocOffset;

    if (ExitStmtVec.size() < 2)
      continue;

    switch (IterationStmt->getStmtClass()) {
    case Stmt::DoStmtClass:
      LocOffset = 1;
      break;
    case Stmt::ForStmtClass:
    case Stmt::CXXForRangeStmtClass:
      LocOffset = 2;
      break;
    case Stmt::WhileStmtClass:
      LocOffset = 4;
      break;
    default:
      LocOffset = 1;
      break;
    }

    auto Loc = IterationStmt->getBeginLoc();
    diag(Loc, "loop contains more than one secondary exit")
        << SourceRange(Loc, Loc.getLocWithOffset(LocOffset))
        << IterationStmt->getStmtClassName();

    for (const Stmt *Stmt : ExitStmtVec) {
      diag(Stmt->getBeginLoc(), "secondary exit here", DiagnosticIDs::Note)
          << Stmt->getSourceRange();
    }
  }
}

void Rule15p4Check::dispatch(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {

  const auto *IterationStmt = Result.Nodes.getNodeAs<Stmt>("stmt");

  Vec.clear();
  SwitchActive = false;

  Vec.emplace_back(IterationStmt);

  for (const auto *Child : IterationStmt->children())
    dispatch(Child);
}

void Rule15p4Check::dispatch(llvm::iterator_range<ConstStmtIterator> Range) {
  for (const auto *Child : Range)
    dispatch(Child);
}

void Rule15p4Check::dispatch(const Stmt *Stmt) {
  if (!Stmt)
    return;

  switch (Stmt->getStmtClass()) {
  case Stmt::CXXForRangeStmtClass:
  case Stmt::DoStmtClass:
  case Stmt::ForStmtClass:
  case Stmt::WhileStmtClass:
    SwitchActive = false;
    Vec.emplace_back(Stmt);

    dispatch(Stmt->children());
    break;
  case Stmt::SwitchStmtClass:
    SwitchActive = true;

    dispatch(Stmt->children());

    SwitchActive = false;
    break;
  default:
    if (const auto *Node = dyn_cast<BreakStmt>(Stmt)) {
      dispatch(Node);
      return;
    }

    if (const auto *Node = dyn_cast<GotoStmt>(Stmt)) {
      dispatch(Node);
      return;
    }

    if (const auto *Node = dyn_cast<IndirectGotoStmt>(Stmt)) {
      dispatch(Node);
      return;
    }

    dispatch(Stmt->children());
    break;
  }
}

void Rule15p4Check::dispatch(const GotoStmt *Stmt) {
  const auto &SM = getSourceManager();
  // Do not emit a diagnostic if the goto does not jump outside of its
  // containing iteration statement and is therefore not terminating it.
  auto Loc = Stmt->getLabel()->getBeginLoc();

  for (const auto *IterationStmt : Vec) {
    auto Range = IterationStmt->getSourceRange();

    if (SM.isPointWithin(Loc, Range.getBegin(), Range.getEnd()))
      continue;

    Map[IterationStmt].emplace_back(Stmt);
  }
}

void Rule15p4Check::dispatch(const IndirectGotoStmt *Stmt) {
  // GNU extension. We cannot know whether this goto will jump outside of the
  // iteration statements or not, so we just assume it does.

  for (const auto *IterationStmt : Vec)
    Map[IterationStmt].emplace_back(Stmt);
}

void Rule15p4Check::dispatch(const BreakStmt *Stmt) {
  if (SwitchActive)
    return;

  Map[Vec.back()].emplace_back(Stmt);
}

} // namespace clang::tidy::misrac
