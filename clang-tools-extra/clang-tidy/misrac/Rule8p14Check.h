//===--- Rule8p14Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P14CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P14CHECK_H

#include "MisracBaseCheck.h"
#include "clang/Lex/Lexer.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(QualType, isRestrictQualified) {
  return Node.isRestrictQualified();
}

} // namespace

/// Detects issues in accordance with MISRA C rule 8.14:
/// "The restrict type qualifier shall not be used".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-8p14.html
class Rule8p14Check : public MisracBaseCheck {
public:
  Rule8p14Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      qualifiedTypeLoc(loc(isRestrictQualified())).bind("type"),
      this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *QTLoc = Result.Nodes.getNodeAs<QualifiedTypeLoc>("type");

    auto PTLoc = QTLoc->getNextTypeLoc().getAs<PointerTypeLoc>();
    if (!PTLoc)
      return;

    // Seems like there is no better way to extract the source location of the
    // restrict qualifier. The QualifiedTypeLoc documentation explicitly states
    // that the source locations of the qualifier are not provided.
    auto MaybeToken = Lexer::findNextToken(
        PTLoc.getStarLoc(), *Result.SourceManager, getLangOpts());

    if (!MaybeToken)
      return;

    auto &Token = MaybeToken.value();
    auto Range = SourceRange(Token.getLocation(), Token.getLastLoc());

    diag(Range.getBegin(), "'restrict' qualifier used")
        << Range << FixItHint::CreateRemoval(Range);
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P14CHECK_H
