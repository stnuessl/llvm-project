//===--- Rule7p1Check.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE7P1CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE7P1CHECK_H

#include "MisracBaseCheck.h"
#include "clang/Lex/Lexer.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(IntegerLiteral, isOctalConstant) {
  const auto &SM = Finder->getASTContext().getSourceManager();
  auto &LangOpts = Finder->getASTContext().getLangOpts();
  auto Loc = SM.getSpellingLoc((&Node)->getBeginLoc());
  SmallString<32> Buffer;

  auto Text = Lexer::getSpelling(Loc, Buffer, SM, LangOpts);

  // An octal literal must start with a '0'.
  if (Text.size() <= 1 || Text[0] != '0')
    return false;

  // The other characters must be octal digits.
  Text = Text.drop_front();
  return llvm::all_of(Text, [](char Ch) { return Ch >= '0' && Ch <= '8'; });
}

} // namespace

/// Detects issues in accordance with MISRA C rule 7.1:
/// "Octal constants shall not be used".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-7p1.html
class Rule7p1Check : public MisracBaseCheck {
public:
  Rule7p1Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    // clang-format off
    Finder->addMatcher(
      integerLiteral(
        isOctalConstant()
      ).bind("stmt"),
      this
    );
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *IL = Result.Nodes.getNodeAs<IntegerLiteral>("stmt");
    auto Range = IL->getSourceRange();

    auto DB = diag(Range.getBegin(), "octal constant used") << Range;

    if (Range.getBegin().isFileID()) {
      auto Buffer = SmallString<32>();
      bool Signed = IL->getType()->isSignedIntegerType();

      IL->getValue().toString(Buffer, 16, Signed,
                              /* FormatAsCLiteral */ true,
                              /* UpperCase */ false,
                              /* InsertSeparators */ false);

      DB << FixItHint::CreateReplacement(Range, Buffer);
    }
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE7P1CHECK_H
