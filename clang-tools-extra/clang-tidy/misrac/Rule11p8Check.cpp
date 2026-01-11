//===--- Rule11p8Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11p8Check.h"

namespace clang::tidy::misrac {

namespace {

QualType getPointeeType(const Expr *Expr) {
  return Expr->getType().getCanonicalType()->getPointeeType();
}

AST_MATCHER(ExplicitCastExpr, dropsCVQualifierFromPointer) {
  const auto *Expr = &Node;

  auto Source = getPointeeType(Expr->getSubExpr());
  if (Source.isNull())
    return false;

  if (!Source.isConstQualified() && !Source.isVolatileQualified())
    return false;

  auto Dest = getPointeeType(Expr);
  if (Dest.isNull())
    return false;

  return !Dest.isConstQualified() || !Dest.isVolatileQualified();
}

} // namespace

void Rule11p8Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  Finder->addMatcher(
      explicitCastExpr(dropsCVQualifierFromPointer()).bind("expr"), this);
}

void Rule11p8Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *Expr = Result.Nodes.getNodeAs<ExplicitCastExpr>("expr");

  auto Dest = getPointeeType(Expr).getQualifiers();
  auto Source = getPointeeType(Expr->getSubExpr()).getQualifiers();
  auto Removed = Source - Dest;

  // Build the diagnostic warning and emit it.
  auto DB = diag(Expr->getExprLoc(), "cast removes %0");

  if (Removed.hasOnlyConst())
    DB << "'const' qualifier";
  else if (Removed.hasOnlyVolatile())
    DB << "'volatile' qualifier";
  else
    DB << "'const' and 'volatile' qualifiers";

  if (const auto *CSCE = dyn_cast<CStyleCastExpr>(Expr))
    DB << SourceRange(CSCE->getLParenLoc(), CSCE->getRParenLoc());

  // Emit an additional note as it might not be obvious how qualifiers are
  // involved due to the use of typedef types.
  diag(Expr->getExprLoc(), "type %0 is casted to %1", DiagnosticIDs::Note)
      << Expr->getSubExpr()->getType() << Expr->getType();
}

} // namespace clang::tidy::misrac
