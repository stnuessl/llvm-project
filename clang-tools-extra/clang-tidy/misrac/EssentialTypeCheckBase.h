//===--- EssentialTypeCheckBase.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_ESSENTIALTYPECHECKBASE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_ESSENTIALTYPECHECKBASE_H

#include "MisracBaseCheck.h"

#include "EssentialType.h"

namespace clang::tidy::misrac {

class EssentialTypeCheckBase : public MisracBaseCheck {
public:
  EssentialTypeCheckBase(StringRef Name, ClangTidyContext *Context);
  virtual ~EssentialTypeCheckBase() = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return !LangOpts.CPlusPlus;
  }

  void addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                      Preprocessor *ModuleExpanderPP) override;
  void addMatchers(ast_matchers::MatchFinder *Finder) override;

  void visitBoolExpansion(SourceLocation Loc);

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;

  EssentialType visit(const ArraySubscriptExpr *Expr);
  EssentialType visit(const BinaryOperator *Expr);
  EssentialType visit(const CallExpr *Expr);
  EssentialType visit(const CharacterLiteral *Expr);
  EssentialType visit(const ConditionalOperator *Expr);
  EssentialType visit(const DeclRefExpr *Expr);
  EssentialType visit(const ExplicitCastExpr *Expr);
  EssentialType visit(const Expr *Expr);
  EssentialType visit(const FullExpr *Expr);
  EssentialType visit(const ImplicitCastExpr *Expr);
  EssentialType visit(const IntegerLiteral *Expr);
  EssentialType visit(const ParenExpr *Expr);
  EssentialType visit(const StringLiteral *Expr);
  EssentialType visit(const RecoveryExpr *Expr);
  EssentialType visit(const UnaryExprOrTypeTraitExpr *Expr);
  EssentialType visit(const UnaryOperator *Expr);

  // Rule 10.1
  virtual void checkUnaryOperand(const UnaryOperator *Expr, EssentialType ET) {}
  virtual void checkBinaryOperands(const BinaryOperator *Expr,
                                   EssentialType ET1, EssentialType ET2) {}
  virtual void checkConditionalOperand(const Expr *Expr, EssentialType ET) {}
  virtual void checkArraySubscript(const Expr *Expr, EssentialType ET) {}

  // Rule 10.2
  virtual void checkAdditiveOpChar(const BinaryOperator *Expr,
                                   EssentialType ET1, EssentialType ET2) {}

  // Rule 10.3
  virtual void checkAssignment(const Expr *Expr, EssentialType ET1,
                               EssentialType ET2) {}
  // Rule 10.4
  virtual void checkOperation(const BinaryOperator *Expr, EssentialType ET1,
                              EssentialType ET2) {}
  virtual void checkOperation(const ConditionalOperator *Expr,
                              EssentialType ET1, EssentialType ET2) {}
  // Rule 10.5
  virtual void checkCast(const ExplicitCastExpr *Expr, EssentialType ET1,
                         EssentialType ET2) {}

  // Rule 10.6
  virtual void checkAssignmentWithComposite(const Expr *Expr, EssentialType ET1,
                                            EssentialType ET2) {}

  // Rule 10.7
  virtual void checkOperationWithComposite(const Expr *Operator,
                                           const Expr *CompositeExpr,
                                           EssentialType ETCompositeExpr,
                                           EssentialType ETOtherOperand) {}

  // Rule 10.8
  virtual void checkCastWithComposite(const ExplicitCastExpr *Expr,
                                      EssentialType ET1, EssentialType ET2) {}

private:
  void dispatch(const Expr *Expr);
  void dispatch(const VarDecl *Decl);
  void dispatch(const FunctionDecl *Decl);
  void dispatch(const ReturnStmt *Stmt);

  void visitAssignment(const Expr *Expr, EssentialType ET1, EssentialType ET2);
  void visitAssignment(const BinaryOperator *Expr, EssentialType ET1,
                       EssentialType ET2);

  void visitOperation(const BinaryOperator *Expr, EssentialType ET1,
                      EssentialType ET2);

  void visitCast(const ExplicitCastExpr *Expr, EssentialType ET1,
                 EssentialType ET2);

  void visitInitExpr(QualType Type, const Expr *Expr);
  void visitInitExprScalar(QualType Type, const Expr *Expr);
  void visitInitExprArray(const ArrayType *Type, const Expr *Expr);
  void visitInitExprRecord(const RecordType *Type, const Expr *Expr);

  std::optional<EssentialType>
  getEssentialTypeIntegerConstantExpr(const Expr *Expr) const;

  EssentialType getEssentialTypeUACOperator(const BinaryOperator *Expr,
                                            EssentialType ET1,
                                            EssentialType ET2) const;

  bool isCompositeExpr(const Expr *Expr) const;
  bool isCompositeExpr(const BinaryOperator *Expr) const;
  bool isCompositeExpr(const ConditionalOperator *Expr) const;

  EssentialType getEssentialType(QualType Type) const;
  EssentialType getEssentialType(const Expr *Expr) const;

  bool isBooleanLocation(SourceLocation Loc) const;
  bool isBooleanType(StringRef Name) const;
  bool isBooleanType(QualType Type) const;

  llvm::DenseSet<SourceLocation> BooleanLocations;
  llvm::DenseSet<StringRef> Booleans;
  QualType ReturnType;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRA_ESSENTIALTYPECHECKBASE_H
