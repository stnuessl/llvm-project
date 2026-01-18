//===--- Rule11p9Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE11P9CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE11P9CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 11.9:
/// "The macro NULL shall be the only permitted form of integer null pointer
/// constant".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-11p9.html
class Rule11p9Check : public MisracBaseCheck {
public:
  Rule11p9Check(StringRef Name, ClangTidyContext *Context);

  void dispatchMacroExpands(const Token &MacroNameTok, SourceRange Range);
  bool isNullExpansionLoc(SourceLocation Loc) const;

protected:
  void addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                      Preprocessor *ModuleExpanderPP) override;

  void addMatchers(ast_matchers::MatchFinder *Finder) override;
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void writeOptions(ClangTidyOptions::OptionMap &Opts) override;
private:
  llvm::DenseSet<SourceLocation> Set;
  std::vector<StringRef> AllowedNullPointerConstants;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE11P9CHECK_H
