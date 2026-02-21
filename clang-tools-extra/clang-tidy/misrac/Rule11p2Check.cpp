//===--- Rule11p2Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11p2Check.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(ImplicitCastExpr, isPartOfExplicitCast) {
    return (&Node)->isPartOfExplicitCast();
}

AST_MATCHER(QualType, isIncompleteType) {
  if (const auto *Type = (&Node)->getTypePtrOrNull())
    return Type->isIncompleteType();

  return false;
}

} // namespace

void Rule11p2Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  auto PointsToIncompleteType = pointsTo(
    qualType(
      hasCanonicalType(qualType(
        isIncompleteType(),
        unless(voidType())
      ))
    ).bind("type")
  );

  Finder->addMatcher(
    castExpr(
      anyOf(
        hasType(PointsToIncompleteType),
        hasSourceExpression(hasType(PointsToIncompleteType))
      ),
      unless(hasCastKind(CK_ToVoid)),
      unless(hasCastKind(CK_NoOp)),
      unless(hasCastKind(CK_NullToPointer)),
      unless(hasCastKind(CK_NullToMemberPointer)),
      unless(implicitCastExpr(isPartOfExplicitCast()))
    ).bind("expr"),
    this);
  // clang-format on
}

void Rule11p2Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *Expr = Result.Nodes.getNodeAs<CastExpr>("expr");
  const auto *Type = Result.Nodes.getNodeAs<QualType>("type");
  SourceRange Range;

  if (const auto *CSEC = dyn_cast<CStyleCastExpr>(Expr))
    Range = SourceRange(CSEC->getLParenLoc(), CSEC->getRParenLoc());

  diag(Expr->getExprLoc(), "cast involving pointer to incomplete type %0")
      << Range << *Type;
}

} // namespace clang::tidy::misrac
