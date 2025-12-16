//===--- Rule15p1Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P1CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P1CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 15.1:
/// "The goto statement should not be used".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-15p1.html
class Rule15p1Check : public MisracBaseCheck {
public:
  Rule15p1Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(gotoStmt().bind("goto"), this);
  }

  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Stmt = Result.Nodes.getNodeAs<GotoStmt>("goto");

    diag(Stmt->getGotoLoc(), "'goto' should not be used")
        << Stmt->getSourceRange();
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P1CHECK_H
