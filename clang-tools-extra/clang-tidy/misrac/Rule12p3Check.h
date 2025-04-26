//===--- Rule12p3Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE12P3CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE12P3CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(BinaryOperator, isCommaOperator) {
  return (&Node)->getOpcode() == BO_Comma;
}

AST_MATCHER(BinaryOperator, any) {
  (void)Node;

  return true;
}

} // namespace

/// Detects issues in accordance with MISRA C rule 12.3:
/// "The comma operator should not be used".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-12p3.html
class Rule12p3Check : public MisracBaseCheck {
public:
  Rule12p3Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context),
        ReportUseInForLoops(Options.get("ReportUseInForLoops", false)) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      binaryOperator(
        isCommaOperator(),
        (ReportUseInForLoops)
          ? any()
          : unless(hasParent(forStmt(hasBody(compoundStmt()))))
      ).bind("op"),
      this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Op = Result.Nodes.getNodeAs<BinaryOperator>("op");

    diag(Op->getExprLoc(), "comma operator used");
  }

  void writeOptions(ClangTidyOptions::OptionMap &Opts) override {
    Options.store(Opts, "ReportUseInForLoops", ReportUseInForLoops);
  }

private:
  bool ReportUseInForLoops;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE12P3CHECK_H
