//===--- MissingElseCheck.h - clang-tidy ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_MISSINGELSECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_MISSINGELSECHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::embedded {

/// Detects 'if' ... 'else if' constructs that lack a terminating 'else'
/// statement. This pattern may be interpreted similarly to a 'switch'
/// statement without a 'default' case.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/embedded/missing-else.html
class MissingElseCheck : public ClangTidyCheck {
public:
  MissingElseCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        StyleName(Options.get("IndentationStyle", "Allman")),
        Style(getIndentationStyle(StyleName)),
        Indentation(Options.get("Indentation", 4)) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override {
    Options.store(Opts, "IndentationStyle", StyleName);
    Options.store(Opts, "Indentation", Indentation);
  }

private:
  enum class IndentationStyle {
    NONE,
    ALLMAN,
    KERNIGHAN_RICHIE,
    UNKNOWN,
  };

  IndentationStyle getIndentationStyle(StringRef Key);

  StringRef StyleName;
  IndentationStyle Style;
  unsigned Indentation;
};

} // namespace clang::tidy::embedded

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_MISSINGELSECHECK_H
