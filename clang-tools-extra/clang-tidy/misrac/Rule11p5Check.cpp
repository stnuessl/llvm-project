//===--- Rule11p5Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11p5Check.h"

namespace clang::tidy::misrac {

void Rule11p5Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  Finder->addMatcher(
    castExpr(
      unless(hasCastKind(CK_NullToPointer)),
      hasSourceExpression(hasType(pointsTo(voidType()))),
      anyOf(
        explicitCastExpr(
          hasDestinationType(unless(pointsTo(voidType())))
        ),
        implicitCastExpr(
          unless(hasImplicitDestinationType(pointsTo(voidType()))),
          unless(hasSourceExpression(ignoringParens(
            explicitCastExpr(hasCastKind(CK_NullToPointer))
          )))
        )
      )
    ).bind("expr"),
    this);
  // clang-format on
}

void Rule11p5Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *Expr = Result.Nodes.getNodeAs<CastExpr>("expr");

  diag(Expr->getExprLoc(), "%0 from %1 to %2")
      << Expr->getSourceRange()
      << ((isa<ImplicitCastExpr>(Expr)) ? "implicit cast" : "cast")
      << Expr->getSubExpr()->getType() << Expr->getType();
}

} // namespace clang::tidy::misrac
