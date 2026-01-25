//===--- Rule20p4Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE20P4CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE20P4CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 20.4:
/// "A macro shall not be deﬁned with the same name as a keyword".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-20p4.html
class Rule20p4Check : public MisracBaseCheck {
public:
  Rule20p4Check(StringRef Name, ClangTidyContext *Context);

  bool isAllowed(StringRef Name) { return llvm::is_contained(Allowed, Name); }
  const LangOptions &getLangOpts() const {
    return MisracBaseCheck::getLangOpts();
  }

protected:
  void addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                      Preprocessor *ModuleExpanderPP) override;

  void writeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  std::vector<StringRef> Allowed;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE20P4CHECK_H
