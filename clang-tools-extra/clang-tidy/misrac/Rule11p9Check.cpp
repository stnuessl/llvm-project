//===--- Rule11p9Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11p9Check.h"
#include "../utils/OptionsUtils.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::misrac {

namespace {

class CallbackManager : public PPCallbacks {
public:
  CallbackManager(Rule11p9Check *Check) : Check(Check) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {

    if (MD.getMacroInfo()->isFunctionLike())
      return;

    Check->dispatchMacroExpands(MacroNameTok, Range);
  }

  Rule11p9Check *Check;
};

AST_MATCHER_P(Expr, isExpandedFromNull, const Rule11p9Check *, Check) {
  return Check->isNullExpansionLoc((&Node)->getExprLoc());
}

AST_MATCHER(ImplicitCastExpr, isPartOfExplicitCast) {
  return (&Node)->isPartOfExplicitCast();
}

} // namespace

Rule11p9Check::Rule11p9Check(StringRef Name, ClangTidyContext *Context)
    : MisracBaseCheck(Name, Context),
      AllowedNullPointerConstants(utils::options::parseStringList(
          Options.get("AllowedNullPointerConstants", "NULL"))) {}

void Rule11p9Check::dispatchMacroExpands(const Token &MacroNameTok,
                                         SourceRange Range) {
  const auto *II = MacroNameTok.getIdentifierInfo();
  if (!II)
    return;

  if (!llvm::is_contained(AllowedNullPointerConstants, II->getName()))
    return;

  auto Loc = getSourceManager().getFileLoc(Range.getBegin());

  (void)Set.insert(Loc);
}

bool Rule11p9Check::isNullExpansionLoc(SourceLocation Loc) const {

  if (!Loc.isMacroID())
    return false;

  Loc = getSourceManager().getFileLoc(Loc);

  return Set.contains(Loc);
}

void Rule11p9Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  Finder->addMatcher(
    implicitCastExpr(
      hasCastKind(CK_NullToPointer),
      unless(hasSourceExpression(ignoringParens(expr(anyOf(
        cxxNullPtrLiteralExpr(),
        // Explicit casts will be checked by the matcher below
        explicitCastExpr(),
        isExpandedFromNull(this)
      ))))),
      unless(isPartOfExplicitCast())
    ).bind("expr"),
    this
  );

  Finder->addMatcher(
    explicitCastExpr(
      hasCastKind(CK_NullToPointer),
      unless(allOf(
        hasType(pointerType(pointee(voidType()))),
        hasSourceExpression(ignoringParens(integerLiteral(
          hasType(isInteger()),
          equals(0)
        )))
      ))
    ).bind("expr"),
    this
  );
  // clang-format on
}

void Rule11p9Check::addPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                   Preprocessor *ModuleExpanderPP) {

  (void)SM;
  (void)ModuleExpanderPP;

  PP->addPPCallbacks(std::make_unique<CallbackManager>(this));
}

void Rule11p9Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  const auto *Match = Result.Nodes.getNodeAs<CastExpr>("expr");
  auto Range = Match->getSourceRange();

  auto DB = diag(Match->getExprLoc(), "forbidden integer null pointer constant")
            << Range;

  if (getLangOpts().CPlusPlus)
    DB << FixItHint::CreateReplacement(Range, "nullptr");

  for (const auto Item : AllowedNullPointerConstants)
    DB << FixItHint::CreateReplacement(Range, Item);

  if (AllowedNullPointerConstants.empty())
    DB << FixItHint::CreateReplacement(Range, "(void *) 0");
}

void Rule11p9Check::writeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(
      Opts, "AllowedNullPointerConstants",
      utils::options::serializeStringList(AllowedNullPointerConstants));
}
} // namespace clang::tidy::misrac
