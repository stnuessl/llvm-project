//===--- MissingStaticCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingStaticCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void MissingStaticCheck::registerMatchers(MatchFinder *Finder) {
  /* clang-format off */
  Finder->addMatcher(
    functionDecl(
      isInline(),
      unless(isStaticStorageClass())
    ).bind("decl"),
    this
  );
  /* clang-format on */
}

void MissingStaticCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("decl");

  diag(Decl->getLocation(), "'static' storage classifier missing from inline "
                            "function declaration '%0'")
      << Decl->getName()
      << FixItHint::CreateInsertion(Decl->getBeginLoc(), "static ");
}

} // namespace clang::tidy::embedded
