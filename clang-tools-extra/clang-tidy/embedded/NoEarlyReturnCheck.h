//===--- NoEarlyReturnCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_NOEARLYRETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_NOEARLYRETURNCHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::embedded {

/// Detects early return statements that are not placed at the end of a
/// function body’s compound statement.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc/no-union.html
class NoEarlyReturnCheck : public ClangTidyCheck {
public:
  NoEarlyReturnCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void onEndOfTranslationUnit() override;

private:
  using ReturnStmtVec = llvm::SmallVector<const ReturnStmt *, 8>;

  std::vector<std::pair<const FunctionDecl *, ReturnStmtVec>> Vec;
};

} // namespace clang::tidy::embedded

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EMBEDDED_NOEARLYRETURNCHECK_H
