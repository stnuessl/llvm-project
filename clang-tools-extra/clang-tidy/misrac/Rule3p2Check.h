//===--- Rule3p2Check.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE3P2CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE3P2CHECK_H

#include "MisracBaseCheck.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 3.2:
/// "Line-splicing shall not be used in // comments".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-3p2.html
class Rule3p2Check : public MisracBaseCheck {
public:
  Rule3p2Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context),
        Checker(std::make_unique<CommentChecker>(this)) {}

  void addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                      Preprocessor *ModuleExpanderPP) override {
    (void)SM;
    (void)ModuleExpanderPP;

    PP->addCommentHandler(Checker.get());
  }

private:
  class CommentChecker : public CommentHandler {
  public:
    CommentChecker(Rule3p2Check *Check) : Check(Check) {}

    bool HandleComment(Preprocessor &PP, SourceRange Range) override {
      auto &SM = PP.getSourceManager();
      StringRef Text = Lexer::getSourceText(
          CharSourceRange::getCharRange(Range), SM, PP.getLangOpts());

      if (!Text.starts_with("//"))
        return false;

      size_t Start = 0;

      while (1) {
        auto Index = Text.find('\\', Start);
        if (Index == StringRef::npos)
          break;

        auto Loc = Range.getBegin().getLocWithOffset(Index);

        Check->diag(Loc, "disallowed '\\' character in // comment");

        // Skip past the found string.
        Start = Index + 1;
      }

      return false;
    }

  private:
    Rule3p2Check *Check;
  };

  std::unique_ptr<CommentChecker> Checker;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE3P2CHECK_H
