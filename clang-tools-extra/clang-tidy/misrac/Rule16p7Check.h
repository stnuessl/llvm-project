//===--- Rule16p7Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P7CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P7CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 16.7:
/// "A switch-expression shall not have essentially Boolean type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-16p7.html
class Rule16p7Check : public EssentialTypeCheckBase {
public:
  Rule16p7Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      switchStmt().bind("stmt"),
      this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Stmt = Result.Nodes.getNodeAs<SwitchStmt>("stmt");
    const auto *Cond = Stmt->getCond();

    auto ET = visit(Cond);
    if (!ET.isBoolean())
      return;

    diag(Cond->getBeginLoc(), "switch condition has boolean value")
        << Cond->getSourceRange();
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P7CHECK_H
