//===--- Rule16p3Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule16p3Check.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misrac {

void Rule16p3Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *Match = Result.Nodes.getNodeAs<SwitchStmt>("switch");
  const auto *Body = Match->getBody();

  if (const auto *Stmt = dyn_cast<CompoundStmt>(Body)) {
    dispatch(Stmt);
    return;
  }

  if (const auto *Stmt = dyn_cast<SwitchCase>(Body)) {
    // The switch seems to consist of only one case statement so we check it
    // directly.
    dispatch(Stmt);
    return;
  }
}

void Rule16p3Check::dispatch(const CompoundStmt *Stmt) {
  if (Stmt->body_empty())
    return;

  auto Range = Stmt->body();
  const auto *It = Range.begin();

  // Check each case for a unconditional terminating break.
  while (It != Range.end()) {
    if (const auto *Item = dyn_cast<SwitchCase>(*It++))
      It = dispatch(It, Range.end(), Item);
  }
}

Stmt *const *Rule16p3Check::dispatch(Stmt *const *Begin, Stmt *const *End,
                                     const SwitchCase *Stmt) {
  // The AST for cases is somewhat complicated. Each case contains
  // one sub statement which might be a compound statement. If there are more
  // statements or expressions below an case, then these appear as successive
  // nodes to the case in the AST. This means we have multiple places to check
  // for the existance of a break statement.
  auto *const *It = Begin;

  while (1) {
    if (It == End || isa<SwitchCase>(*It)) {
      // If existing, the break statement must be part of the sub-statements
      // within the currently evaluated case statement.
      dispatch(Stmt);
      return It;
    }

    if (isa<BreakStmt>(*It))
      return It + 1;

    ++It;
  }
}

void Rule16p3Check::dispatch(const SwitchCase *Stmt) {
  if (const auto *SubStmt = dyn_cast<SwitchCase>(Stmt->getSubStmt())) {
    dispatch(SubStmt);
    return;
  }

  if (hasTerminatingBreak(Stmt))
    return;

  diag(Stmt);
}

bool Rule16p3Check::hasTerminatingBreak(const SwitchCase *Stmt) {
  auto Range = Stmt->children();

  if (Range.empty())
    return false;

  auto Last = Range.begin();

  for (auto It = Last; It != Range.end(); ++It)
    Last = It;

  if (const auto *Stmt = dyn_cast<CompoundStmt>(*Last)) {
    if (Stmt->body_empty())
      return false;

    return isa<BreakStmt>(Stmt->body_back());
  }

  return isa<BreakStmt>(*Last);
}

void Rule16p3Check::diag(const SwitchCase *Case) {
  MisracBaseCheck::diag(Case->getBeginLoc(),
                        "missing unconditional break statement in case")
      << SourceRange(Case->getBeginLoc(), Case->getColonLoc());
}

} // namespace clang::tidy::misrac
