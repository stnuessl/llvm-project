//===--- Rule20p2Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE20P2CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE20P2CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 20.2:
/// "The ', " or \ characters and the /* or // character sequences shall not
/// occur in a header file name".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-20p2.html
class Rule20p2Check : public MisracBaseCheck {
public:
  Rule20p2Check(StringRef Name, ClangTidyContext *Context);

  void dispatchInclusion(StringRef FileName, CharSourceRange FilenameRange);

protected:
  void addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                      Preprocessor *ModuleExpanderPP) override;
  void writeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  DiagnosticBuilder diag(SourceLocation Loc, StringRef Item);

  StringRef RegexPattern;
  llvm::Regex Regex;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE20P2CHECK_H
