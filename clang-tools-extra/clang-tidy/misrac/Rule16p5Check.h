//===--- Rule16p5Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P5CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P5CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 16.5:
/// "A default label shall appear as either the ﬁrst or the last switch label
/// of a switch statement".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-16p5.html
class Rule16p5Check : public MisracBaseCheck {
public:
  Rule16p5Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(switchStmt().bind("switch"), this);
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Stmt = Result.Nodes.getNodeAs<SwitchStmt>("switch");
    const auto *Node = Stmt->getSwitchCaseList();

    if (!Node || isa<DefaultStmt>(Node))
      return;

    Node = Node->getNextSwitchCase();
    if (!Node)
      return;

    while (1) {
      const auto *Next = Node->getNextSwitchCase();
      if (!Next)
        return;

      if (const auto *Stmt = dyn_cast<DefaultStmt>(Node)) {
        ClangTidyCheck::diag(
            Stmt->getDefaultLoc(),
            "the default case must either be the first or last case in a "
            "switch statement")
            << SourceRange(Stmt->getDefaultLoc(), Stmt->getColonLoc());
        break;
      }

      Node = Next;
    }
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P5CHECK_H
