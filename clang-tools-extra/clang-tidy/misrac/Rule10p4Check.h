//===--- Rule10p4Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P4CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P4CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.4:
/// "Both operands of an operator in which the usual arithmetic conversions
/// are performed shall have the same essential type category".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p4.html
class Rule10p4Check : public EssentialTypeCheckBase {
public:
  Rule10p4Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context),
        IgnoreAssignments(Options.get("IgnoreAssignments", false)) {}

  void writeOptions(ClangTidyOptions::OptionMap &Opts) override {
    EssentialTypeCheckBase::storeOptions(Opts);

    Options.store(Opts, "IgnoreAssignments", IgnoreAssignments);
  }

  void checkOperation(const BinaryOperator *Expr, EssentialType ET1,
                      EssentialType ET2) override {

    if (IgnoreAssignments && Expr->isAssignmentOp())
      return;

    visitOperation(Expr, Expr->getExprLoc(), ET1, ET2);
  }

  void checkOperation(const ConditionalOperator *Expr, EssentialType ET1,
                      EssentialType ET2) override {
    visitOperation(Expr, Expr->getColonLoc(), ET1, ET2);
  }

private:
  void visitOperation(const Expr *Expr, SourceLocation Loc, EssentialType ET1,
                      EssentialType ET2) {
    if (!ET1.isSameCategory(ET2)) {
      diag(Loc, "use of different type categories '%0' and '%1' in operation")
          << Expr->getSourceRange() << ET1 << ET2;

      return;
    }

    if (ET1.isEnum() && ET2.isEnum() && ET1 != ET2) {
      diag(Loc, "use of different enumeration types in operation")
          << Expr->getSourceRange();

      return;
    }
  }

  bool IgnoreAssignments;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE10P4CHECK_H
