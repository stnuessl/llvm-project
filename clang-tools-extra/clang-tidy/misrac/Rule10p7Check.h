//===--- Rule10p7Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P7CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P7CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.7:
/// "If a composite expression is used as one operand of an operator in
/// which the usual arithmetic conversions are performed then the other
/// operand shall not have wider essential type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p7.html
class Rule10p7Check : public EssentialTypeCheckBase {
public:
  Rule10p7Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  // FIXME: better interface from base class
  // More AddSourceRange usage throughout the misra checks
  void checkOperationWithComposite(const Expr *Operator,
                                   const Expr *CompositeExpr,
                                   EssentialType ETCompositeExpr,
                                   EssentialType ETOtherOperand) override {

    if (ETOtherOperand.isWider(ETCompositeExpr)) {
      auto Range = CompositeExpr->getSourceRange();

      diag(Range.getBegin(), "narrower %0-bit composite expression used in "
                             "operation with %1-bit operand")
          << Range << ETCompositeExpr.getSize() << ETOtherOperand.getSize();
    }
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P7CHECK_H
