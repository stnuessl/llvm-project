//===--- Rule14p4Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE14P4CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE14P4CHECK_H

#include "EssentialTypeCheckBase.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 14.4:
/// "The controlling expression of an if statement and the controlling
/// expression of an iteration- statement shall have essentially Boolean type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-14p4.html
class Rule14p4Check : public EssentialTypeCheckBase {
public:
  Rule14p4Check(StringRef Name, ClangTidyContext *Context)
      : EssentialTypeCheckBase(Name, Context),
        StrictWhile(Options.get("StrictWhile", false)),
        StrictDo(Options.get("StrictDo", false)) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off

    Finder->addMatcher(
      stmt(anyOf(
        ifStmt().bind("if"),
        forStmt(hasCondition(expr())).bind("for"),
        whileStmt().bind("while"),
        doStmt().bind("do")
      )),
      this);

    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    if (const auto *IS = Result.Nodes.getNodeAs<IfStmt>("if")) {
      dispatch(IS);
      return;
    }

    if (const auto *FS = Result.Nodes.getNodeAs<ForStmt>("for")) {
      dispatch(FS);
      return;
    }

    if (const auto *WS = Result.Nodes.getNodeAs<WhileStmt>("while")) {
      dispatch(WS);
      return;
    }

    if (const auto *DS = Result.Nodes.getNodeAs<DoStmt>("do")) {
      dispatch(DS);
      return;
    }
  }

private:
  void dispatch(const IfStmt *Stmt) {
    const auto *Expr = Stmt->getCond();
    auto ET = visit(Expr);

    if (ET.isBoolean())
      return;

    diagCondition(Expr, ET);
  }

  void dispatch(const ForStmt *Stmt) {
    const auto *Expr = Stmt->getCond();
    auto ET = visit(Expr);

    if (ET.isBoolean())
      return;

    diagCondition(Expr, ET);
  }

  void dispatch(const WhileStmt *Stmt) {
    const auto *Expr = Stmt->getCond();
    auto ET = visit(Expr);

    if (ET.isBoolean())
      return;

    if (!StrictWhile && isIntegerLiteralValue(Expr, 1)) {
      return;
    }

    diagCondition(Expr, ET);
  }

  void dispatch(const DoStmt *Stmt) {
    const auto *Expr = Stmt->getCond();
    auto ET = visit(Expr);

    if (ET.isBoolean())
      return;

    if (!StrictDo && isIntegerLiteralValue(Expr, 0))
      return;

    diagCondition(Expr, ET);
  }

  bool isIntegerLiteralValue(const Expr *Expr, uint32_t Value) {
    Expr = Expr->IgnoreParenImpCasts();

    if (const auto *IL = dyn_cast<IntegerLiteral>(Expr))
      return IL->getValue() == Value;

    return false;
  }

  void diagCondition(const Expr *Expr, EssentialType ET) {
    diag(Expr->getExprLoc(), "inappropriate essential type '%0' for condition")
        << ET << Expr->getSourceRange();

    diag(Expr->getExprLoc(), "conditions must be essentially boolean",
         DiagnosticIDs::Note);
  }

  bool StrictWhile;
  bool StrictDo;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE14P4CHECK_H
