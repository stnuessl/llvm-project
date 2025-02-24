//===--- DefaultCasePositionCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DefaultCasePositionCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void DefaultCasePositionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(switchStmt().bind("switch"), this);
}

void DefaultCasePositionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Stmt = Result.Nodes.getNodeAs<SwitchStmt>("switch");
  const auto *Node = Stmt->getSwitchCaseList();

  if (isa_and_nonnull<DefaultStmt>(Node))
    return;

  Node = Node->getNextSwitchCase();
  const auto *Next = Node->getNextSwitchCase();

  while (Node && Next) {
    if (const auto *Stmt = dyn_cast<DefaultStmt>(Node)) {
      diag(Stmt->getDefaultLoc(),
           "default case must be at first or last position in switch");

      break;
    }

    Node = Next;
    Next = Next->getNextSwitchCase();
  }
}

} // namespace clang::tidy::embedded
