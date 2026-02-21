//===--- Rule11p1Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE11P1CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE11P1CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 11.1:
/// "Conversions shall not be performed between a pointer to a function
/// and any other type".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-11p1.html
class Rule11p1Check : public MisracBaseCheck {
public:
  Rule11p1Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

protected:
  void addMatchers(ast_matchers::MatchFinder *Finder) override;
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE11P1CHECK_H
