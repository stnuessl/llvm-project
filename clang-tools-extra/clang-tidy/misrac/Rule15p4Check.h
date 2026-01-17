//===--- Rule15p4Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P4CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P4CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 15.4:
/// "There should be no more than one break or goto statement used to terminate
/// any iteration statement".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-15p4.html
class Rule15p4Check : public MisracBaseCheck {
public:
  Rule15p4Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void onEndOfTranslationUnit() override;
protected:
  void addMatchers(ast_matchers::MatchFinder *Finder) override;
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;


private:
  void dispatch(llvm::iterator_range<ConstStmtIterator> Range);
  void dispatch(const Stmt *Stmt);
  void dispatch(const GotoStmt *Stmt);
  void dispatch(const IndirectGotoStmt *Stmt);
  void dispatch(const BreakStmt *Stmt);

  // Map iteration statements to break and gotos contained within them.
  llvm::DenseMap<const Stmt *, llvm::SmallVector<const Stmt *, 8>> Map;
  llvm::SmallVector<const Stmt *, 8> Vec;
  bool SwitchActive;

};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE15P4CHECK_H
