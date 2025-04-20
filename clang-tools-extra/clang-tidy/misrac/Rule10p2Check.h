//===--- Rule10p2Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P2CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P2CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.2:
/// "Expressions of essentially  character type shall not be used
/// inappropriately in addition and subtraction operations".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p2.html
class Rule10p2Check : public EssentialTypeCheckBase {
public:
  Rule10p2Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  void checkAdditiveOpChar(const BinaryOperator *Expr, EssentialType ET1,
                           EssentialType ET2) override {

    switch (Expr->getOpcode()) {
    case BO_Add:
    case BO_AddAssign:
      break;
    case BO_Sub:
    case BO_SubAssign:
      if (ET1.isChar() && ET2.isChar())
        return;
      break;
    default:
      return;
    }

    diag(Expr->getExprLoc(),
         "'%0' operation uses essential types '%1' and '%2'")
        << Expr->getSourceRange() << Expr->getOpcodeStr() << ET1 << ET2;
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P1CHECK_H
