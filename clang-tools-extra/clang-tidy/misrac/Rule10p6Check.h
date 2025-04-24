//===--- Rule10p6Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P6CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P6CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.6:
/// "The value of a composite expression shall not be assigned to an object
/// with wider essential type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p6.html
class Rule10p6Check : public EssentialTypeCheckBase {
public:
  Rule10p6Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  void checkAssignmentWithComposite(const Expr *Expr, EssentialType ET1,
                                    EssentialType ET2) override {

    if (ET1.isWider(ET2)) {
      SourceRange Range;
      SourceLocation Loc;

      const auto *BO = dyn_cast<BinaryOperator>(Expr->IgnoreImpCasts());
      if (BO && BO->isAssignmentOp()) {
        Range = BO->getSourceRange();
        Loc = BO->getExprLoc();
      } else {
        // Expression is a variable initializer, function argument or a return
        // statement.
        Range = Expr->getSourceRange();
        Loc = Range.getBegin();
      }

      diag(Loc,
           "assignment of %0-bit composite expression to wider %1-bit type")
          << Range << ET2.getSize() << ET1.getSize();
    }
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P6CHECK_H
