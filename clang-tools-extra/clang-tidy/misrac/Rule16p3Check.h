//===--- Rule16p3Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P3CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P3CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 16.3:
/// "An unconditional break statement shall terminate every switch-clause".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-16p3.html
class Rule16p3Check : public MisracBaseCheck {
public:
  Rule16p3Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(switchStmt().bind("switch"), this);
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void dispatch(const CompoundStmt *Stmt);
  Stmt *const *dispatch(Stmt *const *Begin, Stmt *const *End,
                        const SwitchCase *Stmt);
  void dispatch(const SwitchCase *Stmt);
  bool hasTerminatingBreak(const SwitchCase *Stmt);
  void diag(const SwitchCase *Case);
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P3CHECK_H
