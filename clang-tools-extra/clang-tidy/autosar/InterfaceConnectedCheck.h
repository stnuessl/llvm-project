//===--- InterfaceConnectedCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_INTERFACECONNECTEDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_INTERFACECONNECTEDCHECK_H

#include "../ClangTidyCheck.h"

namespace clang::tidy::autosar {

/// Checks automatically generated AUTOSAR interfaces and emits appropriate
/// diagnostics if unconnected interfaces are detected.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/autosar/interface-connected.html
class InterfaceConnectedCheck : public ClangTidyCheck {
public:
  InterfaceConnectedCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context), SrcM(nullptr), MacroInterfaceMap(64),
        UnconnectedMacroLocations(64), MacroRegex(getMacroRegexPattern()),
        FunctionRegex(getFunctionRegexPattern()) {

    std::string ErrMsg;

    if (!MacroRegex.isValid(ErrMsg)) {
      configurationDiag("option 'MacroRegex': invalid regex: '%0'",
                        DiagnosticIDs::Error)
          << ErrMsg;
      ErrMsg.clear();
    }

    if (!FunctionRegex.isValid(ErrMsg)) {
      configurationDiag("option 'FunctionRegex': invalid regex: '%0'",
                        DiagnosticIDs::Error)
          << ErrMsg;
      ErrMsg.clear();
    }
  }

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.C99 || LangOpts.C11 || LangOpts.C17;
  }

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override {
    Options.store(Opts, "MacroRegex", getMacroRegexPattern());
    Options.store(Opts, "FunctionRegex", getFunctionRegexPattern());
  }

  bool addUnconnectedMacroLocation(SourceLocation Loc) {
    Loc = SrcM->getFileLoc(Loc);

    auto [It, Ok] = UnconnectedMacroLocations.insert(Loc);
    return Ok || It != UnconnectedMacroLocations.end();
  }

  bool addMacroInterface(SourceLocation Loc, StringRef Name) {
    Loc = SrcM->getFileLoc(Loc);

    auto [It, Ok] = MacroInterfaceMap.insert({Loc, Name});
    return Ok || It != MacroInterfaceMap.end();
  }

  bool isUnconnectedMacroLocation(SourceLocation Loc) const {
    Loc = SrcM->getFileLoc(Loc);

    return UnconnectedMacroLocations.contains(Loc);
  }

  bool isMacroInterface(StringRef Name) const { return MacroRegex.match(Name); }

  bool isMacroInterface(SourceLocation Loc) const {
    Loc = SrcM->getFileLoc(Loc);

    return MacroInterfaceMap.contains(Loc);
  }

  void internalDiag(SourceLocation Loc, StringRef Entity) {
    diag(Loc, "internal: failed to process '%0'") << Entity;
  }

private:
  StringRef getMacroInterfaceName(SourceLocation Loc) {
    Loc = SrcM->getFileLoc(Loc);

    auto It = MacroInterfaceMap.find(Loc);
    if (It == MacroInterfaceMap.end())
      return "(null)";

    return It->second;
  }

  StringRef getMacroRegexPattern(void) const {
    return Options.get("MacroRegex", "Rte_.+");
  }

  StringRef getFunctionRegexPattern(void) const {
    return Options.get("FunctionRegex", "Rte_.+");
  }

  const SourceManager *SrcM;
  llvm::DenseMap<SourceLocation, StringRef> MacroInterfaceMap;
  llvm::DenseSet<SourceLocation> UnconnectedMacroLocations;
  llvm::Regex MacroRegex;
  llvm::Regex FunctionRegex;
};

} // namespace clang::tidy::autosar

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOSAR_INTERFACECONNECTEDCHECK_H
