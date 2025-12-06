//===--- Rule17p12Check.h - clang-tidy --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P12CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P12CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(UnaryOperator, isAddressOperator) {
  return (&Node)->getOpcode() == UO_AddrOf;
}

AST_MATCHER(ImplicitCastExpr, isFunctionToPointerDecay) {
  return (&Node)->getCastKind() == CastKind::CK_FunctionToPointerDecay;
}

} // namespace

/// Detects issues in accordance with MISRA C rule 17.12:
/// "A function identifier should only be used with either a preceding &, or
/// with a parenthesized parameter list".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-17p12.html
class Rule17p12Check : public MisracBaseCheck {
public:
  Rule17p12Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    /* clang-format off */
    Finder->addMatcher(
      declRefExpr(
        to(functionDecl()),
        unless(
          anyOf(
            hasParent(unaryOperator(isAddressOperator())),
            hasParent(
              implicitCastExpr(
                isFunctionToPointerDecay(),
                hasParent(callExpr())
              )
            )
          )
        )
      ).bind("expr"),
      this);
    /* clang-format on */
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Expr = Result.Nodes.getNodeAs<DeclRefExpr>("expr");
    auto Loc = Expr->getExprLoc();

    diag(Loc, "missing address operator")
        << FixItHint::CreateInsertion(Loc, "&");
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P12CHECK_H
