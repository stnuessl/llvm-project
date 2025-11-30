//===--- Rule16p4Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P4CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P4CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 16.4:
/// "Every switch statement shall have a default label."
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-16p4.html
class Rule16p4Check : public MisracBaseCheck {
public:
  Rule16p4Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(
        switchStmt(unless(hasDescendant(defaultStmt()))).bind("switch"), this);
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Stmt = Result.Nodes.getNodeAs<SwitchStmt>("switch");

    auto InsertionLoc = Stmt->getEndLoc();
    auto Column = Result.SourceManager->getSpellingColumnNumber(InsertionLoc);

    SmallString<32> Code;
    Code.append("default:\n");
    Code.append(Column - 1, ' ');

    diag(Stmt->getSwitchLoc(), "missing 'default' label")
        << FixItHint::CreateInsertion(InsertionLoc, Code);
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE16P4CHECK_H
