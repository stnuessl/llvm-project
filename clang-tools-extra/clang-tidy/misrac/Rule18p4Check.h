//===--- Rule18p4Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P4CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P4CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 18.4:
/// "The +, -, += and -= operators should not be applied to an expression of
/// pointer type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-18p4.html
class Rule18p4Check : public MisracBaseCheck {
public:
  Rule18p4Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      binaryOperator(
        hasAnyOperatorName("+", "-", "+=", "-="),
        hasType(pointerType())
      ).bind("bo"),
      this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *BO = Result.Nodes.getNodeAs<BinaryOperator>("bo");
    SourceRange Range;

    if (BO->getLHS()->getType()->isPointerType())
      Range = BO->getLHS()->getSourceRange();
    else
      Range = BO->getRHS()->getSourceRange();

    diag(Range.getBegin(), "pointer used in '%0' operation")
        << BO->getOpcodeStr() << Range;
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P4CHECK_H
