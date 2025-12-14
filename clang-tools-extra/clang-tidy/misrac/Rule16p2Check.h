//===--- Rule16p2Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P2CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P2CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 16.2:
/// "A switch label shall only be used when the most closely-enclosing compound
/// statement is the body of a switch statement".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-16p2.html
class Rule16p2Check : public MisracBaseCheck {
public:
  Rule16p2Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      switchCase(
        unless(hasParent(stmt(anyOf(
          compoundStmt(hasParent(switchStmt())),
          switchCase()
        ))))
      ).bind("stmt"),
      this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Match = Result.Nodes.getNodeAs<SwitchCase>("stmt");

    diag(Match->getBeginLoc(), "invalid enclosing parent of switch case")
        << SourceRange(Match->getBeginLoc(), Match->getColonLoc());
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P2CHECK_H
