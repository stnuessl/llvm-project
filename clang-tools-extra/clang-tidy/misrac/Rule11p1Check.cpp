//===--- Rule11p1Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11p1Check.h"

namespace clang::tidy::misrac {

void Rule11p1Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  auto PointsToFPT = pointsTo(hasCanonicalType(functionProtoType()));

  // clang-format off
  Finder->addMatcher(
    castExpr(anyOf(
      allOf(
        hasType(PointsToFPT),
        unless(hasSourceExpression(hasType(PointsToFPT))),
        unless(hasCastKind(CK_NullToPointer)),
        unless(hasCastKind(CK_NullToMemberPointer)),
        unless(hasCastKind(CK_FunctionToPointerDecay)),
        unless(implicitCastExpr(hasSourceExpression(
          explicitCastExpr(hasCastKind(CK_NullToPointer))
        )))),
      allOf(
        hasSourceExpression(hasType(PointsToFPT)),
        unless(hasType(PointsToFPT)),
        unless(hasCastKind(CK_ToVoid))
      ))
    ).bind("expr"),
    this);
  // clang-format on
}

void Rule11p1Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *Expr = Result.Nodes.getNodeAs<CastExpr>("expr");
  const auto *SubExpr = Expr->getSubExpr();
  SourceRange Range;

  if (const auto *CSCE = dyn_cast<CStyleCastExpr>(Expr))
    Range = SourceRange(CSCE->getLParenLoc(), CSCE->getRParenLoc());

  diag(Expr->getExprLoc(), "cast involving %0 and %1")
      << Range << Expr->getType() << SubExpr->getType();
}

} // namespace clang::tidy::misrac
