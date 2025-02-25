//===--- NoVariableLengthArrayCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoVariableLengthArrayCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void NoVariableLengthArrayCheck::registerMatchers(MatchFinder *Finder) {
  // clang-format off

  Finder->addMatcher(
    varDecl(hasType(variableArrayType())).bind("decl"),
    this);

  // clang-format on
}

void NoVariableLengthArrayCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *Decl = Result.Nodes.getNodeAs<VarDecl>("decl");

    diag(Decl->getLocation(), "forbidden use of variadic length array");
}

} // namespace clang::tidy::embedded
