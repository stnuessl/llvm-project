//===--- Rule15p2Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P2CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P2CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(GotoStmt, isBackwardJumping) {
  return (&Node)->getBeginLoc() > (&Node)->getLabel()->getLocation();
}

} // namespace

/// Detects issues in accordance with MISRA C rule 15.2:
/// "The goto statement shall jump to a label declared later in the same
/// function".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-15p2.html
class Rule15p2Check : public MisracBaseCheck {
public:
  Rule15p2Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(gotoStmt(isBackwardJumping()).bind("goto"), this);
  }

  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Stmt = Result.Nodes.getNodeAs<GotoStmt>("goto");

    diag(Stmt->getGotoLoc(), "backward jumping goto") << Stmt->getSourceRange();

    diag(Stmt->getLabel()->getBeginLoc(), "label defined here",
         DiagnosticIDs::Note);
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P2CHECK_H
