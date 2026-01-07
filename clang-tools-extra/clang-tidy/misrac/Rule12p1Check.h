//===--- Rule12p1Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE12P1CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE12P1CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 12.1:
/// "The precedence of operators within expressions should be made explicit".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-12p1.html
class Rule12p1Check : public MisracBaseCheck {
public:
  Rule12p1Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    // C++ has more operators than described in the MISRA C standard.
    return !LangOpts.CPlusPlus;
  }

protected:
  void addMatchers(ast_matchers::MatchFinder *Finder) override;
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void dispatch(const BinaryOperator *Expr);
  void dispatch(const ConditionalOperator *Expr);
  void dispatch(const UnaryExprOrTypeTraitExpr *Expr);
  void dispatchOperand(const Expr *Expr, unsigned Precedence);
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE12P1CHECK_H
