//===--- Rule17p1Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule17p1Check.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/STLExtras.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misrac {

namespace {

constexpr StringRef StdargMacros[] = {
    "va_arg",
    "va_copy",
    "va_end",
    "va_start",
};

class CallbackManager : public PPCallbacks {
public:
  CallbackManager(Rule17p1Check *Check) : Check(Check) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    const auto *Info = MD.getMacroInfo();

    (void)Args;

    if (!Info->isFunctionLike())
      return;

    auto Name = MacroNameTok.getIdentifierInfo()->getName();

    if (Name.empty())
      return;

    if (!llvm::is_contained(StdargMacros, Name))
      return;

    Check->diag(Range.getBegin(), "use of macro '%0'") << Name << Range;
  }

private:
  Rule17p1Check *Check;
};

AST_MATCHER(QualType, decayedVaList) {
  // Isn't there a better way to do this?
  const auto *DType = Node->getAs<DecayedType>();

  if (!DType)
    return false;

  const auto *EType = DType->getOriginalType()->getAs<ElaboratedType>();
  if (!EType)
    return false;

  auto NType = EType->getNamedType();
  if (NType.isNull())
    return false;

  const auto *TType = NType->getAs<TypedefType>();
  if (!TType)
    return false;

  const auto *Decl = TType->getDecl();
  if (!Decl)
    return false;

  return Decl->getName() == "va_list";
}

} // namespace

void Rule17p1Check::addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                   Preprocessor *ModuleExpanderPP) {
  (void)SM;
  (void)ModuleExpanderPP;

  PP->addPPCallbacks(std::make_unique<CallbackManager>(this));
}

void Rule17p1Check::addMatchers(MatchFinder *Finder) {
  // clang-format off
  Finder->addMatcher(
    varDecl(
      anyOf(
        hasType(asString("va_list")),
        parmVarDecl(hasType(decayedVaList()))
      )
    ).bind("decl"),
    this);
  // clang-format on
}

void Rule17p1Check::dispatch(const MatchFinder::MatchResult &Result) {
  const auto *Decl = Result.Nodes.getNodeAs<VarDecl>("decl");

  diag(Decl->getBeginLoc(), "use of type 'va_list'") << Decl->getSourceRange();
}

} // namespace clang::tidy::misrac
