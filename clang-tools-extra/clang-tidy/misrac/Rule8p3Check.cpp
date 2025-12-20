//===--- Rule8p3Check.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8p3Check.h"

using namespace clang::ast_matchers;

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(FunctionDecl, isSimpleFunction) {
  return (&Node)->getKind() == Decl::Function;
}

AST_POLYMORPHIC_MATCHER(isFirstDecl,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(VarDecl,
                                                        FunctionDecl)) {
  return (&Node)->isFirstDecl();
}

SourceRange getTypeSourceRange(const DeclaratorDecl *Decl) {
  if (const auto *TSI = Decl->getTypeSourceInfo())
    return TSI->getTypeLoc().getSourceRange();

  return SourceRange(Decl->getBeginLoc());
}

} // namespace

void Rule8p3Check::addMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(unless(isFirstDecl())).bind("var"), this);

  // clang-format off
  Finder->addMatcher(
    functionDecl(
      isSimpleFunction(),
      unless(isFirstDecl())
    ).bind("fn"),
    this);
  // clang-format on
}

void Rule8p3Check::dispatch(const MatchFinder::MatchResult &Result) {
  if (const auto *Decl = Result.Nodes.getNodeAs<VarDecl>("var")) {
    dispatch(Decl);
    return;
  }

  if (const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("fn")) {
    dispatch(Decl);
    return;
  }
}

void Rule8p3Check::dispatch(const VarDecl *Decl) {
  dispatch(Decl->getFirstDecl(), Decl);
}

void Rule8p3Check::dispatch(const FunctionDecl *Decl) {
  dispatchReturnType(Decl);
  dispatchParameters(Decl);
}

void Rule8p3Check::dispatchReturnType(const FunctionDecl *Decl) {
  auto FirstType = Decl->getFirstDecl()->getReturnType();
  if (Decl->getReturnType() == FirstType)
    return;

  auto Range = Decl->getReturnTypeSourceRange();

  emitWarning(Range, FirstType);
  emitNoteReturnType(Decl->getFirstDecl(), Decl);
}

void Rule8p3Check::dispatchParameters(const FunctionDecl *Decl) {
  auto FirstParams = Decl->getFirstDecl()->parameters();
  auto Params = Decl->parameters();

  if (FirstParams.size() != Params.size())
    return;

  for (auto [FirstDecl, Decl] : llvm::zip(FirstParams, Params))
    dispatch(FirstDecl, Decl);
}

void Rule8p3Check::dispatch(const DeclaratorDecl *FirstDecl,
                            const DeclaratorDecl *Decl) {
  if (FirstDecl == Decl)
    return;

  if (FirstDecl->getType() == Decl->getType())
    return;

  auto Range = getTypeSourceRange(Decl);

  emitWarning(Range, FirstDecl->getType());
  emitNote(FirstDecl, Decl);
}

void Rule8p3Check::emitWarning(SourceRange Range, QualType FirstType) {
  diag(Range.getBegin(), "type mismatch with primary declaration")
      << Range << createTypeReplacement(Range, FirstType);
}

void Rule8p3Check::emitNote(const DeclaratorDecl *FirstDecl,
                            const ValueDecl *Decl) {
  auto Range = getTypeSourceRange(FirstDecl);

  diag(Range.getBegin(), "declaration here uses %0 and not %1",
       DiagnosticIDs::Note)
      << Range << FirstDecl->getType() << Decl->getType();
}

void Rule8p3Check::emitNoteReturnType(const FunctionDecl *FirstDecl,
                                      const FunctionDecl *Decl) {
  auto FirstRange = FirstDecl->getReturnTypeSourceRange();
  diag(FirstRange.getBegin(), "declaration here uses %0 and not %1",
       DiagnosticIDs::Note)
      << FirstRange << FirstDecl->getReturnType() << Decl->getReturnType();
}

} // namespace clang::tidy::misrac
