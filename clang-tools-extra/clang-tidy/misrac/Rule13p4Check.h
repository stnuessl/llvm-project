//===--- Rule13p4Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE13P4CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE13P4CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_POLYMORPHIC_MATCHER(hasUnusedResult,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(BinaryOperator,
                                                        ParenExpr)) {
  using namespace clang::ast_matchers;

  // clang-format off
  auto ParentNotUsingResult = hasParent(stmt(anyOf(
    compoundStmt(),
    mapAnyOf(
      doStmt,
      forStmt,
      ifStmt,
      whileStmt,
      switchStmt
    ).with(
      unless(hasCondition(equalsNode(&Node)))
    ),
    switchCase(),
    binaryOperator(
      hasOperatorName(","),
      hasLHS(equalsNode(&Node))
    ),
    parenExpr(hasUnusedResult())
  )));
  // clang-format on

  if (BinaryOperator::classof(&Node))
    return binaryOperator(ParentNotUsingResult).matches(Node, Finder, Builder);

  if (ParenExpr::classof(&Node))
    return parenExpr(ParentNotUsingResult).matches(Node, Finder, Builder);

  llvm_unreachable("Not a valid polymorphic type");
}

} // namespace

/// Detects issues in accordance with MISRA C rule 13.4:
/// "The result of an assignment operator should not be used".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-13p4.html
class Rule13p4Check : public MisracBaseCheck {
public:
  Rule13p4Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      binaryOperator(
        isAssignmentOperator(),
        unless(hasUnusedResult())
      ).bind("bo"),
      this
    );
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *BO = Result.Nodes.getNodeAs<BinaryOperator>("bo");

    auto DB = diag(BO->getExprLoc(), "result of assignment used")
              << BO->getSourceRange();

    if (BO->getOpcode() == BO_Assign)
      DB << FixItHint::CreateReplacement(BO->getExprLoc(), "==");
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE13P4CHECK_H
