//===--- Rule3p1Check.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE3P1CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE3P1CHECK_H

#include "MisracBaseCheck.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 3.1:
/// "The character sequences /* an d // shall not be used within a comment".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-3p1.html
class Rule3p1Check : public MisracBaseCheck {
public:
  Rule3p1Check(StringRef Name, ClangTidyContext *Context)
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
    CommentChecker(Rule3p1Check *Check) : Check(Check) {}

    bool HandleComment(Preprocessor &PP, SourceRange Range) override {
      auto &SM = PP.getSourceManager();
      StringRef Text = Lexer::getSourceText(
          CharSourceRange::getCharRange(Range), SM, PP.getLangOpts());

      auto Strings = llvm::SmallVector<StringRef, 2>();

      if (Text.consume_front("/*"))
        Strings.append({"/*", "//"});
      else if (Text.consume_front("//"))
        Strings.emplace_back("/*");
      else
        return false;

      for (StringRef Item : Strings) {
        size_t Start = 0;

        while (1) {
          auto Index = Text.find(Item, Start);
          if (Index == StringRef::npos)
            break;

          // Add an offset of 2 because we dropped the start of the comment.
          auto Loc = Range.getBegin().getLocWithOffset(2 + Index);

          Check->diag(Loc, "disallowed character sequence '%0' in comment")
              << Item
              << SourceRange(Loc, Loc.getLocWithOffset(Item.size() - 1));

          // Skip past the found string.
          Start = Index + Item.size();
        }
      }

      return false;
    }

  private:
    Rule3p1Check *Check;
  };

  std::unique_ptr<CommentChecker> Checker;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE3P1CHECK_H
