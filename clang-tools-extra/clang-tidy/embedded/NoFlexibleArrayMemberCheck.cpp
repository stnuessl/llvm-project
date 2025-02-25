//===--- NoFlexibleArrayMemberCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoFlexibleArrayMemberCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void NoFlexibleArrayMemberCheck::registerMatchers(MatchFinder *Finder) {
  // clang-format off
  Finder->addMatcher(
    fieldDecl(
      hasType(incompleteArrayType())
    ).bind("decl"),
    this);
  // clang-format on
}

void NoFlexibleArrayMemberCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *Decl = Result.Nodes.getNodeAs<FieldDecl>("decl");

    diag(Decl->getLocation(), "forbidden use of flexible array member");
}

} // namespace clang::tidy::embedded
