//===--- Rule17p8Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P8CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P8CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 17.8:
/// "A function parameter should not be modified".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-17p8.html
class Rule17p8Check : public MisracBaseCheck {
public:
  Rule17p8Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context),
        AddrOpDiagLevel(
            asDiagLevel(Options.get(OptionKeyAddrOpDiagLevel, "Warning"))) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    // The check cannot deal with C++ references.
    return !LangOpts.CPlusPlus;
  }

  void addMatchers(ast_matchers::MatchFinder *Finder) override;

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void writeOptions(ClangTidyOptions::OptionMap &Opts) override {
    auto Value = Options.get(OptionKeyAddrOpDiagLevel, "Warning");

    Options.store(Opts, OptionKeyAddrOpDiagLevel, Value);
  }

private:
  constexpr static StringRef OptionKeyAddrOpDiagLevel = "AddrOpDiagLevel";

  DiagnosticIDs::Level asDiagLevel(StringRef Value) {
    if (Value == "Warning")
      return DiagnosticIDs::Warning;

    if (Value == "Remark")
      return DiagnosticIDs::Remark;

    if (Value == "Ignored")
      return DiagnosticIDs::Ignored;

    configurationDiag("invalid configuration value '%0'", DiagnosticIDs::Error)
        << Value;

    return DiagnosticIDs::Warning;
  }

  DiagnosticIDs::Level AddrOpDiagLevel;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P8CHECK_H
