//===--- MisracBaseCheck.h - clang-tidy -------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_MISRACBASECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_MISRACBASECHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::misrac {

class MisracBaseCheck : public ClangTidyCheck {
public:
  MisracBaseCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context),
        ExtendToCPlusPlus(Options.get("ExtendToCPlusPlus", true)),
        ReportAtMacroDefinition(Options.get("ReportAtMacroDefinition", false)) {
  }

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return !LangOpts.CPlusPlus || ExtendToCPlusPlus;
  }

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) final {
    SrcM = &SM;

    addPPCallbacks(SM, PP, ModuleExpanderPP);
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) final {
    addMatchers(Finder);
  }

  void storeOptions(ClangTidyOptions::OptionMap &Opts) final {
    Options.store(Opts, "ExtendToCPlusPlus", ExtendToCPlusPlus);
    Options.store(Opts, "ReportAtMacroDefinition", ReportAtMacroDefinition);

    writeOptions(Opts);
  }

  void check(const ast_matchers::MatchFinder::MatchResult &Result) final {
    Context = Result.Context;
    SrcM = Result.SourceManager;

    dispatch(Result);
  }

  DiagnosticBuilder diag(SourceLocation Loc, StringRef Description,
                         DiagnosticIDs::Level Level = DiagnosticIDs::Warning) {
    if (ReportAtMacroDefinition)
      Loc = SrcM->getSpellingLoc(Loc);

    return ClangTidyCheck::diag(Loc, Description, Level);
  }

protected:
  virtual void addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                              Preprocessor *ModuleExpanderPP) {}
  virtual void addMatchers(ast_matchers::MatchFinder *Finder) {}
  virtual void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) {}
  virtual void writeOptions(ClangTidyOptions::OptionMap &Opts) {}

  const ASTContext &getASTContext() const { return *Context; }
  const SourceManager &getSourceManager() const { return *SrcM; }

private:
  const ASTContext *Context = nullptr;
  const SourceManager *SrcM = nullptr;

  bool ExtendToCPlusPlus = false;
  bool ReportAtMacroDefinition = false;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_MISRACBASECHECK_H
