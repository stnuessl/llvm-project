//===--- Rule10p1Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P1CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P1CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 10.1:
/// "Operands shall not be of an inappropriate essential type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-10p1.html
class Rule10p1Check : public EssentialTypeCheckBase {
public:
  Rule10p1Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context) {}

  void checkUnaryOperand(const UnaryOperator *Expr, EssentialType ET) override {
    switch (Expr->getOpcode()) {
    case UO_Plus:
      if (ET.isInteger() || ET.isFloating())
        return;

      emitDiag(Expr->getSubExpr(), "essential type '%0' in unary plus", ET);

      break;
    case UO_Minus:
      if (ET.isSigned() || ET.isFloating())
        return;

      emitDiag(Expr->getSubExpr(), "essential type '%0' in unary minus", ET);

      break;
    case UO_PostInc:
    case UO_PostDec:
    case UO_PreInc:
    case UO_PreDec:
      visitOperandAddSub(Expr->getSubExpr(), ET);
      break;
    case UO_LNot:
      visitOperandLogical(Expr->getSubExpr(), ET);
      break;
    case UO_Not:
      visitOperandBitwise(Expr->getSubExpr(), ET);
      break;
    default:
      break;
    }
  }

  void checkBinaryOperands(const BinaryOperator *Expr, EssentialType ET1,
                           EssentialType ET2) override {

    switch (Expr->getOpcode()) {
    case BO_Add:
    case BO_Sub:
    case BO_AddAssign:
    case BO_SubAssign:
      visitOperandAddSub(Expr->getLHS(), ET1);
      visitOperandAddSub(Expr->getRHS(), ET2);
      break;
    case BO_Mul:
    case BO_Div:
    case BO_MulAssign:
    case BO_DivAssign:
      visitOperandMulDiv(Expr->getLHS(), ET1);
      visitOperandMulDiv(Expr->getRHS(), ET2);
      break;
    case BO_Rem:
    case BO_RemAssign:
      visitOperandRem(Expr->getLHS(), ET1);
      visitOperandRem(Expr->getRHS(), ET2);
      break;
    case BO_GE:
    case BO_GT:
    case BO_LE:
    case BO_LT:
      visitOperandRelational(Expr->getLHS(), ET1);
      visitOperandRelational(Expr->getRHS(), ET2);
      break;
    case BO_LAnd:
    case BO_LOr:
      visitOperandLogical(Expr->getLHS(), ET1);
      visitOperandLogical(Expr->getRHS(), ET2);
      break;
    case BO_Shl:
    case BO_Shr:
    case BO_ShlAssign:
    case BO_ShrAssign:
      visitOperandShift(Expr->getLHS(), ET1);
      visitOperandShift(Expr->getRHS(), ET2);
      break;
    case BO_And:
    case BO_Xor:
    case BO_Or:
      visitOperandBitwise(Expr->getLHS(), ET1);
      visitOperandBitwise(Expr->getRHS(), ET2);
      break;
    default:
      break;
    }
  }

  virtual void checkConditionalOperand(const Expr *Expr,
                                       EssentialType ET) override {
    if (ET.isBoolean())
      return;

    emitDiag(Expr, "essential type '%0' in conditional operation", ET);
    diag(Expr->getExprLoc(), "operand must be essentially boolean",
         DiagnosticIDs::Note);
  }

  void checkArraySubscript(const Expr *Expr, EssentialType ET) override {
    if (ET.isInteger() || ET.isEnum())
      return;

    emitDiag(Expr, "essential type '%0' in array subscript expression", ET);

    diag(Expr->getExprLoc(),
         "operand must be essentially signed, unsigned or enum",
         DiagnosticIDs::Note);
  }

private:
  void visitOperandAddSub(const Expr *Expr, EssentialType ET) {
    if (ET.isChar() || ET.isInteger() || ET.isFloating())
      return;

    emitDiag(Expr, "essential type '%0' in arithmetic operation", ET);

    diag(Expr->getExprLoc(),
         "operand must be essentially char, signed, unsigned or floating",
         DiagnosticIDs::Note);
  }

  void visitOperandMulDiv(const Expr *Expr, EssentialType ET) {
    if (ET.isInteger() || ET.isFloating())
      return;

    emitDiag(Expr, "essential type '%0' in arithmetic operation", ET);

    diag(Expr->getExprLoc(),
         "operand must be essentially signed, unsigned or floating",
         DiagnosticIDs::Note);
  }

  void visitOperandRem(const Expr *Expr, EssentialType ET) {
    if (ET.isInteger())
      return;

    emitDiag(Expr, "essential type '%0' in modulo operation", ET);

    diag(Expr->getExprLoc(), "operand must be essentially signed or unsigned",
         DiagnosticIDs::Note);
  }

  void visitOperandRelational(const Expr *Expr, EssentialType ET) {
    if (!ET.isBoolean())
      return;

    emitDiag(Expr, "essential type '%0' in relational operation", ET);
  }

  void visitOperandLogical(const Expr *Expr, EssentialType ET) {
    if (ET.isBoolean())
      return;

    emitDiag(Expr, "essential type '%0' in logical operation", ET);
    diag(Expr->getExprLoc(), "operand must be essentially boolean",
         DiagnosticIDs::Note);
  }

  void visitOperandShift(const Expr *Expr, EssentialType ET) {
    if (ET.isUnsigned())
      return;

    emitDiag(Expr, "essential type '%0' in shift operation", ET);

    diag(Expr->getExprLoc(), "operand must be essentially unsigned",
         DiagnosticIDs::Note);
  }

  void visitOperandBitwise(const Expr *Expr, EssentialType ET) {
    if (ET.isUnsigned())
      return;

    emitDiag(Expr, "essential type '%0' in bitwise operation", ET);

    diag(Expr->getExprLoc(), "operand must be essentially unsigned",
         DiagnosticIDs::Note);
  }

  void emitDiag(const Expr *Expr, StringRef Message, EssentialType ET) {
    diag(Expr->getExprLoc(), Message) << Expr->getSourceRange() << ET;
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_RULE10P1CHECK_H
