//===--- Rule12p1Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule12p1Check.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::misrac {

namespace {

// Operators with a precedence greater than 12 won't raise any warnings, so
// we use this value to filter them out. In practice, this value should never
// be used.
constexpr unsigned PrecedenceMax = ~0U;

AST_MATCHER(UnaryExprOrTypeTraitExpr, hasParens) {
  if ((&Node)->isArgumentType())
    return true;

  return (&Node)->getArgumentExpr()->getStmtClass() == Stmt::ParenExprClass;
}

unsigned getPrecedence(const UnaryOperator *Op) {
  switch (Op->getOpcode()) {
  case UO_PostDec:
  case UO_PostInc:
    return 15;
  case UO_AddrOf:
  case UO_Deref:
  case UO_LNot:
  case UO_Minus:
  case UO_Not:
  case UO_Plus:
  case UO_PreDec:
  case UO_PreInc:
    return 14;
  default:
    return PrecedenceMax;
  }
}

unsigned getPrecedence(const BinaryOperator *Op) {
  switch (Op->getOpcode()) {
  case BO_Mul:
  case BO_Div:
  case BO_Rem:
    return 12;
  case BO_Add:
  case BO_Sub:
    return 11;
  case BO_Shl:
  case BO_Shr:
    return 10;
  case BO_GT:
  case BO_GE:
  case BO_LT:
  case BO_LE:
    return 9;
  case BO_EQ:
  case BO_NE:
    return 8;
  case BO_And:
    return 7;
  case BO_Xor:
    return 6;
  case BO_Or:
    return 5;
  case BO_LAnd:
    return 4;
  case BO_LOr:
    return 3;
  case BO_Assign:
  case BO_MulAssign:
  case BO_DivAssign:
  case BO_RemAssign:
  case BO_AddAssign:
  case BO_SubAssign:
  case BO_ShlAssign:
  case BO_ShrAssign:
  case BO_AndAssign:
  case BO_XorAssign:
  case BO_OrAssign:
    return 1;
  case BO_Comma:
    return 0;
  default:
    return PrecedenceMax;
  }
}

unsigned getPrecedence(const Expr *Expr) {
  Expr = Expr->IgnoreImpCasts();

  switch (Expr->getStmtClass()) {
  case Stmt::CXXBoolLiteralExprClass:
  case Stmt::CXXNullPtrLiteralExprClass:
  case Stmt::CharacterLiteralClass:
  case Stmt::FixedPointLiteralClass:
  case Stmt::FloatingLiteralClass:
  case Stmt::ImaginaryLiteralClass:
  case Stmt::IntegerLiteralClass:
  case Stmt::StringLiteralClass:
  case Stmt::UserDefinedLiteralClass:
  case Stmt::DeclRefExprClass:
  case Stmt::ParenExprClass:
    return 16;
  case Stmt::ArraySubscriptExprClass:
  case Stmt::CallExprClass:
  case Stmt::CompoundLiteralExprClass:
  case Stmt::MemberExprClass:
    return 15;
  case Stmt::UnaryExprOrTypeTraitExprClass:
    return 14;
  case Stmt::CStyleCastExprClass:
    return 13;
  case Stmt::ConditionalOperatorClass:
    return 2;
  case Stmt::UnaryOperatorClass:
    return getPrecedence(dyn_cast<UnaryOperator>(Expr));
  case Stmt::BinaryOperatorClass:
    return getPrecedence(dyn_cast<BinaryOperator>(Expr));
  default:
    return PrecedenceMax;
  }
}

} // namespace

void Rule12p1Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  Finder->addMatcher(binaryOperator().bind("bo"), this);
  Finder->addMatcher(conditionalOperator().bind("co"), this);

  // clang-format off
  Finder->addMatcher(
    unaryExprOrTypeTraitExpr(
      ofKind(UETT_SizeOf),
      unless(hasParens())
    ).bind("sizeof"),
    this);
  // clang-format on
}

void Rule12p1Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  if (const auto *Match = Result.Nodes.getNodeAs<BinaryOperator>("bo")) {
    dispatch(Match);
    return;
  }

  if (const auto *Match = Result.Nodes.getNodeAs<ConditionalOperator>("co")) {
    dispatch(Match);
    return;
  }

  if (const auto *Match =
          Result.Nodes.getNodeAs<UnaryExprOrTypeTraitExpr>("sizeof")) {

    dispatch(Match);
    return;
  }
}

void Rule12p1Check::dispatch(const BinaryOperator *Expr) {
  auto Precedence = getPrecedence(Expr);
  if (Precedence < 2)
    return;

  dispatchOperand(Expr->getLHS(), Precedence);
  dispatchOperand(Expr->getRHS(), Precedence);
}

void Rule12p1Check::dispatch(const ConditionalOperator *Expr) {
  auto Precedence = getPrecedence(Expr);

  dispatchOperand(Expr->getCond(), Precedence);
  dispatchOperand(Expr->getTrueExpr(), Precedence);
  dispatchOperand(Expr->getFalseExpr(), Precedence);
}

void Rule12p1Check::dispatch(const UnaryExprOrTypeTraitExpr *Expr) {
  const auto *ArgExpr = Expr->getArgumentExpr();

  auto Range = Expr->getSourceRange();

  StringRef Text = Lexer::getSourceText(
      CharSourceRange::getTokenRange(ArgExpr->getSourceRange()),
      getSourceManager(), getLangOpts());

  auto ReplacementText = llvm::SmallString<64>();
  ReplacementText.append("sizeof(");
  ReplacementText.append(Text);
  ReplacementText.push_back(')');

  diag(ArgExpr->getExprLoc(), "missing parentheses")
      << Range << FixItHint::CreateReplacement(Range, ReplacementText);
}

void Rule12p1Check::dispatchOperand(const Expr *Expr, unsigned Precedence) {
  auto ExprPrecedence = getPrecedence(Expr);

  if (ExprPrecedence > 12)
    return;

  if (ExprPrecedence <= Precedence)
    return;

  auto Range = Expr->getSourceRange();

  StringRef Text = Lexer::getSourceText(CharSourceRange::getTokenRange(Range),
                                        getSourceManager(), getLangOpts());

  auto ReplacementText = llvm::SmallString<64>();
  ReplacementText.push_back('(');
  ReplacementText.append(Text);
  ReplacementText.push_back(')');

  diag(Expr->getExprLoc(), "implicit operator precedence")
      << Range << FixItHint::CreateReplacement(Range, ReplacementText);
}

} // namespace clang::tidy::misrac
