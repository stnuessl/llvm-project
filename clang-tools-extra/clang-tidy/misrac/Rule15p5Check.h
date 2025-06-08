//===--- Rule15p5Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P5CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P5CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 15.5:
/// "A function should have a single point of exit at the end".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-15p5.html
class Rule15p5Check : public MisracBaseCheck {
public:
  Rule15p5Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    // The implementation does not allow for nested functions which is
    // effectively a valid language construct in C++.
    return !LangOpts.CPlusPlus;
  }

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(functionDecl().bind("decl"), this);
    Finder->addMatcher(returnStmt().bind("stmt"), this);
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {

    if (const auto *Match = Result.Nodes.getNodeAs<FunctionDecl>("decl")) {
      dispatch();
      Decl = Match;
      return;
    }

    if (const auto *Match = Result.Nodes.getNodeAs<ReturnStmt>("stmt")) {
      Stmts.push_back(Match);
      return;
    }
  }

  void onEndOfTranslationUnit() override { dispatch(); }

private:
  void dispatch() {
    if (!Decl)
      return;

    if (Stmts.empty())
      return;

    // Make sure the top-down ordering of the return statements is the same
    // as in the source code.
    constexpr auto Compare = [](const ReturnStmt *A, const ReturnStmt *B) {
      return A->getBeginLoc() < B->getBeginLoc();
    };

    llvm::sort(Stmts, Compare);

    // At most a single return statement at the very end of a function is
    // allowed.
    const auto *Stmt = dyn_cast<CompoundStmt>(Decl->getBody());
    if (!Stmt->body_empty() && Stmt->body_back() == Stmts.back())
      Stmts.pop_back();

    // Since the compliant case has already been dealt with, every remaining
    // return statement is non-compliant.
    for (const auto *ReturnStmt : Stmts)
      diag(ReturnStmt->getReturnLoc(), "early return statement");

    Stmts.clear();
  }

  const FunctionDecl *Decl;
  llvm::SmallVector<const ReturnStmt *, 8> Stmts;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P5CHECK_H
