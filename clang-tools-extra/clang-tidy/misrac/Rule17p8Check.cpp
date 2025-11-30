//===--- Rule17p8Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule17p8Check.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(MemberExpr, hasDereferencingBase) {
  const auto *Expr = (&Node)->getBase()->IgnoreParenImpCasts();

  if (const auto *UO = dyn_cast<UnaryOperator>(Expr))
    return UO->getOpcode() == UO_Deref;

  return false;
}

AST_MATCHER(Expr, isModified) {
  using namespace clang::ast_matchers;
  const auto *Expr = &Node;

  // clang-format off
  return expr(
    hasParent(expr(anyOf(
      unaryOperator(
        hasAnyOperatorName("--", "++"),
        hasUnaryOperand(equalsNode(Expr))
      ),
      binaryOperator(
        isAssignmentOperator(),
        hasLHS(equalsNode(Expr))
      ),
      castExpr(isModified()),
      parenExpr(isModified())
      )))
    ).matches(Node, Finder, Builder);
  // clang-format on
}

AST_MATCHER(Expr, isUsedInAddrOf) {
  using namespace clang::ast_matchers;

  return expr(
    hasParent(expr(anyOf(
      unaryOperator(
        hasOperatorName("&"),
        unless(hasParent(expr(
          hasType(pointerType(pointee(isConstQualified())))
        )))
      ).bind("addr"),
      castExpr(isUsedInAddrOf()),
      parenExpr(isUsedInAddrOf())
    )))
  ).matches(Node, Finder, Builder);
}

} // namespace

void Rule17p8Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  Finder->addMatcher(
    expr(anyOf(
      declRefExpr(
        to(parmVarDecl().bind("param")),
        unless(hasType(isConstQualified())),
        anyOf(isModified(), isUsedInAddrOf())
      ).bind("ref"),
      memberExpr(
        unless(isArrow()),
        unless(hasDereferencingBase()),
        hasObjectExpression(ignoringParenCasts(
          declRefExpr(
            to(parmVarDecl(unless(hasType(pointerType()))).bind("param")),
            unless(hasType(isConstQualified()))
          ).bind("ref"))),
        anyOf(isModified(), isUsedInAddrOf())
      )
    )),
    this);
  // clang-format on
}

void Rule17p8Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *Decl = Result.Nodes.getNodeAs<ParmVarDecl>("param");
  const auto *Expr = Result.Nodes.getNodeAs<DeclRefExpr>("ref");

  if (const auto *Op = Result.Nodes.getNodeAs<UnaryOperator>("addr")) {
    diag(Op->getOperatorLoc(),
         "use of address operator may lead to unintentional modification of "
         "parameter '%0'",
         AddrOpDiagLevel)
        << Decl->getName() << Op->getSourceRange();
    return;
  }

  diag(Expr->getExprLoc(), "forbidden modification of parameter '%0'")
      << Decl->getName() << Expr->getSourceRange();

  diag(Decl->getLocation(), "parameter declared here", DiagnosticIDs::Note)
      << Decl->getSourceRange();
}

} // namespace clang::tidy::misrac
