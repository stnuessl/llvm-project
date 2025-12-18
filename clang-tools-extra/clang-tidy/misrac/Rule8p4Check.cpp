//===--- Rule8p4Check.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8p4Check.h"
#include "clang/Lex/Lexer.h"

#include "../utils/Matchers.h"
#include "../utils/OptionsUtils.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(VarDecl, isLocalVarDeclOrParm) {
  return (&Node)->isLocalVarDeclOrParm();
}

AST_MATCHER(FunctionDecl, isSimpleFunction) {
  return (&Node)->getKind() == Decl::Function;
}

AST_POLYMORPHIC_MATCHER(hasStaticStorage,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(VarDecl,
                                                        FunctionDecl)) {
  return (&Node)->getStorageClass() == SC_Static;
}

AST_POLYMORPHIC_MATCHER(isFirstDecl,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(VarDecl,
                                                        FunctionDecl)) {
  return (&Node)->isFirstDecl();
}

} // namespace

Rule8p4Check::Rule8p4Check(StringRef Name, ClangTidyContext *Context)
    : MisracBaseCheck(Name, Context),
      IgnoreFunctions(utils::options::parseStringList(
          Options.get("IgnoreFunctions", "^::main$"))) {}

void Rule8p4Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  Finder->addMatcher(
    varDecl(
      isDefinition(),
      isFirstDecl(),
      unless(hasStaticStorage()),
      unless(isLocalVarDeclOrParm())
    ).bind("var"),
    this);

  Finder->addMatcher(
    functionDecl(
      isSimpleFunction(),
      isDefinition(),
      isFirstDecl(),
      unless(hasStaticStorage()),
      unless(matchers::matchesAnyListedName(IgnoreFunctions))
    ).bind("fn"),
    this);
  // clang-format on
}

void Rule8p4Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {
  if (const auto *Decl = Result.Nodes.getNodeAs<VarDecl>("var")) {
    auto EndLoc = Lexer::getLocForEndOfToken(
        Decl->getLocation(), 0, Decl->getASTContext().getSourceManager(),
        getLangOpts());

    diag(Decl->getLocation(), "missing declaration for variable definition")
        << SourceRange(Decl->getLocation(), EndLoc)
        << FixItHint::CreateInsertion(Decl->getBeginLoc(), "static ");
    return;
  }

  if (const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("fn")) {
    diag(Decl->getLocation(), "missing declaration for function definition")
        << Decl->getNameInfo().getSourceRange()
        << FixItHint::CreateInsertion(Decl->getBeginLoc(), "static ");
    return;
  }
}

void Rule8p4Check::writeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "IgnoreFunctions",
                utils::options::serializeStringList(IgnoreFunctions));
}

} // namespace clang::tidy::misrac
