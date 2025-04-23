//===--- Rule10p3Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P3CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P3CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.3:
/// "The value of an expression shall not be assigned to an object with a
/// narrower essential type or of a different essential type category".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p3.html
class Rule10p3Check : public EssentialTypeCheckBase {
public:
  Rule10p3Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  void checkAssignment(const Expr *Expr, EssentialType ET1,
                       EssentialType ET2) override {

    if (!ET1.isSameCategory(ET2)) {
      diag(Expr,
           "use of different type categories '%0' and '%1' in assignement")
          << ET1 << ET2;

      return;
    }

    if (ET1.isEnum() && ET2.isEnum() && ET1 != ET2) {
      diag(Expr, "assignment to different enumeration type");
      return;
    }

    if (ET1.isNarrower(ET2)) {
      diag(Expr, "assignment of %0-bit value to narrower %1-bit value")
          << ET2.getSize() << ET1.getSize();

      return;
    }
  }

private:
  DiagnosticBuilder diag(const Expr *Expr, StringRef Message) {
    if (const auto *BO = dyn_cast<BinaryOperator>(Expr->IgnoreImpCasts())) {
      if (BO->isAssignmentOp()) {
        return EssentialTypeCheckBase::diag(BO->getExprLoc(), Message)
               << BO->getSourceRange();
      }
    }

    // We are dealing with a variable initialization, function parameter
    // passing or return value.
    auto Range = Expr->getSourceRange();

    // Make sure the diagnostic is not filtered because the underlying
    // expression stems from a system header, like for example the macros
    // "false" and "true".
    auto Loc = getSourceManager().getFileLoc(Range.getBegin());

    return EssentialTypeCheckBase::diag(Loc, Message) << Range;
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P3CHECK_H
