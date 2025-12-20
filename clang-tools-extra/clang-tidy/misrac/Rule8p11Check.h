//===--- Rule8p11Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P11CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P11CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(VarDecl, hasExternalStorage) {
  return (&Node)->hasExternalStorage();
}

} // namespace

/// Detects issues in accordance with MISRA C rule 8.11:
/// "When an array with external linkage is declared, its size should be
/// explicitly specified".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-8p11.html
class Rule8p11Check : public MisracBaseCheck {
public:
  Rule8p11Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      varDecl(
        hasExternalStorage(),
        hasType(incompleteArrayType())
      ).bind("decl"),
      this
    );
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Decl = Result.Nodes.getNodeAs<VarDecl>("decl");

    diag(Decl->getLocation(), "missing size specification")
        << Decl->getSourceRange();
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P11CHECK_H
