//===--- NoVariableArrayCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoVariableArrayCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misc {

namespace {

AST_MATCHER(QualType, isVariableArray) { return Node->isVariableArrayType(); }

} // namespace

void NoVariableArrayCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasType(isVariableArray())).bind("decl"), this);
}

void NoVariableArrayCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Decl = Result.Nodes.getNodeAs<VarDecl>("decl");

  diag(Decl->getLocation(), "disallowed use of variable length array '%0'")
      << Decl->getName() << FixItHint::CreateRemoval(Decl->getSourceRange());
}

} // namespace clang::tidy::misc
