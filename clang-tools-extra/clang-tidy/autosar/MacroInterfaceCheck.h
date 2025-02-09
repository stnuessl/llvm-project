//===--- MacroInterfaceCheck.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_MACROINTERFACECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_MACROINTERFACECHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::autosar {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/autosar/macro-interface.html
class MacroInterfaceCheck : public ClangTidyCheck {
public:
  MacroInterfaceCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.C99 || LangOpts.C11 || LangOpts.C17;
  }

  void setSwcName(StringRef Name) {
    if (!SwcName.empty())
      return;

    SwcName = Name;
  }

  void addMacro(StringRef Name) { Macros.push_back(Name); }

  bool addInterface(SourceLocation Loc, StringRef Name) {
    Loc = SrcM->getFileLoc(Loc);

    auto [It, Ok] = InterfaceMap.insert({Loc, Name});
    return Ok || It != InterfaceMap.end();
  }

  bool isMacroInterfaceLocation(SourceLocation Loc) const {
    Loc = SrcM->getFileLoc(Loc);

    return InterfaceMap.count(Loc);
  }

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const SourceManager *SrcM;
  llvm::DenseMap<SourceLocation, StringRef> InterfaceMap;
  std::vector<StringRef> Macros;
  StringRef SwcName;
};

} // namespace clang::tidy::autosar

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_MACROINTERFACECHECK_H
