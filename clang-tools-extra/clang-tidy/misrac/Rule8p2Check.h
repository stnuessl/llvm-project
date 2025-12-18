//===--- Rule8p2Check.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P2CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P2CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(ParmVarDecl, isUnnamed) { return (&Node)->getName() == ""; }

AST_MATCHER(FunctionDecl, hasPrototype) { return (&Node)->hasPrototype(); }

} // namespace

/// Detects issues in accordance with MISRA C rule 8.2:
/// "Function types shall be in prototype form with named parameters".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-8p2.html
class Rule8p2Check : public MisracBaseCheck {
public:
  Rule8p2Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      functionDecl(
        unless(isDefinition()),
        unless(hasPrototype())
      ).bind("func"),
      this);

    Finder->addMatcher(
      parmVarDecl(
        isUnnamed(),
        unless(hasParent(functionDecl(unless(isDefinition()))))
      ).bind("param"),
      this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    if (const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
      auto Range = Decl->getFunctionTypeLoc().getParensRange();

      diag(Range.getBegin(), "missing function prototype")
          << Range << FixItHint::CreateReplacement(Range, "(void)");

      return;
    }
    if (const auto *Decl = Result.Nodes.getNodeAs<ParmVarDecl>("param")) {
      diag(Decl->getBeginLoc(), "disallowed unnamed parameter")
          << Decl->getSourceRange();
      return;
    }
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P2CHECK_H
