//===--- Rule6p3Check.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule6p3Check.h"

namespace clang::tidy::misrac {

void Rule6p3Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  Finder->addMatcher(
    fieldDecl(
      hasParent(recordDecl(isUnion()).bind("record")),
      isBitField()
    ).bind("field"),
    this);
  // clang-format on
}

void Rule6p3Check::dispatch(const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FieldDecl>("field");
  const auto *RD = Result.Nodes.getNodeAs<RecordDecl>("record");

  diag(FD->getLocation(), "bit field '%0' declared as member of a union")
      << FD->getSourceRange() << FD->getName();

  diag(RD->getLocation(), "union declared here", DiagnosticIDs::Note);
}

} // namespace clang::tidy::misrac
