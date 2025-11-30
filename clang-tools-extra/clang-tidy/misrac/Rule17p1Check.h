//===--- Rule17p1Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P1CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P1CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 17.1:
/// "The features of <stdarg.h> shall not be used".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-17p1.html
class Rule17p1Check : public MisracBaseCheck {
public:
  Rule17p1Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                      Preprocessor *ModuleExpanderPP) override;
  void addMatchers(ast_matchers::MatchFinder *Finder) override;

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE17P1CHECK_H
