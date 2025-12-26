//===--- Rule7p3Check.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE7P3CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE7P3CHECK_H

#include "MisracBaseCheck.h"
#include "clang/Lex/Lexer.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(IntegerLiteral, hasLowercaseLSuffix) {
  auto &SM = Finder->getASTContext().getSourceManager();
  auto &LangOpts = Finder->getASTContext().getLangOpts();
  auto Loc = SM.getSpellingLoc((&Node)->getBeginLoc());
  SmallString<32> Buffer;

  auto Text = Lexer::getSpelling(Loc, Buffer, SM, LangOpts);

  for (auto Ch : llvm::reverse(Text)) {
    if (std::isdigit(Ch))
      return false;

    if (Ch == 'l')
      return true;
  }

  return false;
}

} // namespace

/// Detects issues in accordance with MISRA C rule 7.3:
/// "The lowercase character 'l' shall not be used in a literal suffix".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-7p3.html
class Rule7p3Check : public MisracBaseCheck {
public:
  Rule7p3Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      integerLiteral(
        hasLowercaseLSuffix()
      ).bind("stmt"),
      this
    );
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *IL = Result.Nodes.getNodeAs<IntegerLiteral>("stmt");
    auto Range = IL->getSourceRange();
    const auto &SM = getSourceManager();

    auto DB = diag(Range.getBegin(),
                   "disallowed use of lowercase 'l' in literal suffix")
              << Range;

    if (Range.getBegin().isFileID()) {
      auto Loc = SM.getSpellingLoc(Range.getBegin());
      SmallString<32> Buffer;

      (void)Lexer::getSpelling(Loc, Buffer, SM, getLangOpts());

      std::replace(Buffer.begin(), Buffer.end(), 'l', 'L');

      DB << FixItHint::CreateReplacement(
          SourceRange(Loc, SM.getSpellingLoc(IL->getEndLoc())), Buffer);
    }
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE7P3CHECK_H
