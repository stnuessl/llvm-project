//===--- NoUnionCheck.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoUnionCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Transformer/SourceCode.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void NoUnionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(tagDecl(isUnion()).bind("union"), this);
}

void NoUnionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Decl = Result.Nodes.getNodeAs<TagDecl>("union");

  diag(Decl->getLocation(), "disallowed declaration of union");
}

} // namespace clang::tidy::embedded
