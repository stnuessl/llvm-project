//===--- Rule15p7Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P7CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P7CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 15.7:
/// "All if ... else if constructs shall be terminated with an else statement".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-15p7.html
class Rule15p7Check : public MisracBaseCheck {
public:
  Rule15p7Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      ifStmt(
        hasElse(ifStmt(unless(hasElse(anything()))))
      ).bind("Stmt"),
      this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Stmt = Result.Nodes.getNodeAs<IfStmt>("Stmt");

    diag(Stmt->getElseLoc(), "'else if' not terminated by 'else'");
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P7CHECK_H
