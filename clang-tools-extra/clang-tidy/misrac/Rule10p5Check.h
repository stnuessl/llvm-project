//===--- Rule10p5Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P5CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P5CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.5:
/// "The value of an expression should not be cast to an inappropriate
/// essential type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p2.html
class Rule10p5Check : public EssentialTypeCheckBase {
public:
  Rule10p5Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  void checkCast(const ExplicitCastExpr *Expr, EssentialType ET1,
                 EssentialType ET2) override {
    if (ET2.hasAppropriateCastTo(ET1))
      return;

    if (ET1.isBoolean() && isBooleanValue(Expr->getSubExpr()))
      return;

    if (ET1.isEnum() && ET2.isEnum()) {
      diag(Expr, "essential type category cast to different enumeration type");
      return;
    }

    diag(Expr, "essential type category cast from '%0' to '%1'") << ET2 << ET1;
  }

private:
  DiagnosticBuilder diag(const ExplicitCastExpr *Expr, StringRef Message) {
    auto Begin = Expr->getExprLoc();
    auto End = Expr->getSubExpr()->getExprLoc();

    return EssentialTypeCheckBase::diag(Begin, Message)
           << CharSourceRange::getCharRange(Begin, End);
  }

  static bool isBooleanValue(const Expr *Expr) {
    Expr = Expr->IgnoreParenImpCasts();

    if (const auto *IL = dyn_cast<IntegerLiteral>(Expr))
      return isBooleanValue(IL);

    return false;
  }

  static bool isBooleanValue(const IntegerLiteral *Expr) {
    const auto &Value = Expr->getValue();

    return Value == 0 || Value == 1;
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P5CHECK_H
