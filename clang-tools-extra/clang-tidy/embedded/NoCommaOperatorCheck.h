//===--- NoCommaOperatorCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_NOCOMMAOPERATORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_NOCOMMAOPERATORCHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::embedded {

/// Detects usage of the comma operator, which is considered bad practice
/// according to coding standards like MISRA C.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/embedded/no-comma-operator.html
class NoCommaOperatorCheck : public ClangTidyCheck {
public:
  NoCommaOperatorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        StrictMode(Options.get("StrictMode", false)),
        ReportAtMacroExpansion(Options.get("ReportAtMacroExpansion", false)) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override {
    Options.store(Opts, "StrictMode", StrictMode);
    Options.store(Opts, "ReportAtMacroExpansion", ReportAtMacroExpansion);
  }

private:
  bool StrictMode;
  bool ReportAtMacroExpansion;
};

} // namespace clang::tidy::embedded

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_NOCOMMAOPERATORCHECK_H
