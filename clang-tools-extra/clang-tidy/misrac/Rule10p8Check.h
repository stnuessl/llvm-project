//===--- Rule10p8Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P8CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P8CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.8:
/// "The value of a composite expression shall not be cast to a different
/// essential type category or a wider essential type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p8.html
class Rule10p8Check : public EssentialTypeCheckBase {
public:
  Rule10p8Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  void checkCastWithComposite(const ExplicitCastExpr *Expr, EssentialType ET1,
                              EssentialType ET2) override {

    if (!ET1.isSameCategory(ET2)) {
      diag(Expr, "essential type category cast of composite expression from "
                 "'%0' to '%1'")
          << ET2 << ET1;
      return;
    }

    if (ET1.isEnum() && ET2.isEnum() && ET1 != ET2) {
      // Given the type promotion rules of the essential type system, it seems
      // like this branch here will never be reached. However, it is still
      // provided for completeness.
      diag(Expr, "essential type category cast of composite expression to "
                 "different enumeration type");

      return;
    }

    if (ET1.isWider(ET2)) {
      diag(Expr, "essential type category cast of %0-bit composite expression "
                 "to wider %1-bit type")
          << ET2.getSize() << ET1.getSize();

      return;
    }
  }

private:
  DiagnosticBuilder diag(const ExplicitCastExpr *Expr, StringRef Message) {
    auto Begin = Expr->getBeginLoc();
    auto End = Expr->getSubExpr()->getEndLoc();

    return EssentialTypeCheckBase::diag(Expr->getExprLoc(), Message)
           << CharSourceRange::getTokenRange(Begin, End);
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P8CHECK_H
