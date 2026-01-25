//===--- Rule20p4Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule20p4Check.h"
#include "../utils/OptionsUtils.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::misrac {

namespace {
class MacroDefinedHandler : public PPCallbacks {
public:
  explicit MacroDefinedHandler(Rule20p4Check *Check) : Check(Check) {}
  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {

    (void)MD;

    const auto *II = MacroNameTok.getIdentifierInfo();
    if (!II)
      return;

    if (!II->isKeyword(Check->getLangOpts()))
      return;

    if (Check->isAllowed(II->getName()))
      return;

    auto Loc = MacroNameTok.getLocation();

    Check->diag(Loc, "macro defined with same name as keyword '%0'")
        << SourceRange(Loc, MacroNameTok.getLastLoc()) << II->getName();
  }

private:
  Rule20p4Check *Check;
};

} // namespace

Rule20p4Check::Rule20p4Check(StringRef Name, ClangTidyContext *Context)
    : MisracBaseCheck(Name, Context),
      Allowed(utils::options::parseStringList(Options.get("Allowed", ""))) {}

void Rule20p4Check::addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                   Preprocessor *ModuleExpanderPP) {
  (void)SM;
  (void)ModuleExpanderPP;

  PP->addPPCallbacks(std::make_unique<MacroDefinedHandler>(this));
}

void Rule20p4Check::writeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "Allowed", utils::options::serializeStringList(Allowed));
}

} // namespace clang::tidy::misrac
