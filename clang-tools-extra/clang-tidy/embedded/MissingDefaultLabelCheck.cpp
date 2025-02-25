//===--- MissingDefaultLabelCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingDefaultLabelCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void MissingDefaultLabelCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      switchStmt(unless(hasDescendant(defaultStmt()))).bind("switch"), this);
}

void MissingDefaultLabelCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Stmt = Result.Nodes.getNodeAs<SwitchStmt>("switch");

  auto InsertionLoc = Stmt->getEndLoc();
  auto Column = Result.SourceManager->getSpellingColumnNumber(InsertionLoc);

  SmallString<32> Code;
  Code.append("default: break;\n");
  Code.append(Column - 1, ' ');

  diag(Stmt->getSwitchLoc(), "'switch' missing 'default' label")
      << FixItHint::CreateInsertion(InsertionLoc, Code);
}

} // namespace clang::tidy::embedded
