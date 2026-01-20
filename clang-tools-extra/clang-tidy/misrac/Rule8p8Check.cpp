//===--- Rule8p8Check.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8p8Check.h"

namespace clang::tidy::misrac {

namespace {

AST_POLYMORPHIC_MATCHER(isFirstDecl,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(VarDecl,
                                                        FunctionDecl)) {
  return (&Node)->isFirstDecl();
}

AST_POLYMORPHIC_MATCHER(hasDifferentDeclWithNonStaticStorageClass,
                        AST_POLYMORPHIC_SUPPORTED_TYPES(VarDecl,
                                                        FunctionDecl)) {
  return llvm::any_of((&Node)->redecls(), [](const auto &&Decl) {
    return Decl->getStorageClass() != SC_Static;
  });
}

AST_MATCHER(FunctionDecl, isSimpleFunction) {
  // The static specifier has a different meaning on C++ methods
  return (&Node)->getKind() == Decl::Function;
}

SourceRange getNameSourceRange(const FunctionDecl *Decl) {
  //
  // Range of the function prototype, e.g.
  //
  //    extern void f(void) {}
  //           ^^^^^^^^^^^^
  //
  return SourceRange(Decl->getTypeSpecStartLoc(), Decl->getTypeSpecEndLoc());
}

SourceRange getNameSourceRange(const VarDecl *Decl) {
  return Decl->getSourceRange();
}

FixItHint createFixItHint(const FunctionDecl *Decl) {
  if (!Decl->isThisDeclarationADefinition())
    return FixItHint::CreateRemoval(Decl->getSourceRange());

  // There cannot be an "extern" linkage specifier on a definition
  return FixItHint::CreateInsertion(Decl->getBeginLoc(), "static ");
}

FixItHint createFixItHint(const VarDecl *Decl) {
  // As there was previously a declaration with the "static" storage class,
  // we can remove any other redeclaration after that.
  return FixItHint::CreateRemoval(Decl->getSourceRange());
}

template <typename T>
void dispatchDecl(Rule8p8Check *Check, const T *FirstDecl) {
  for (const auto *Decl : FirstDecl->redecls()) {
    if (Decl->getStorageClass() == SC_Static)
      continue;

    Check->diag(Decl->getLocation(),
                "'static' missing from declaration with internal linkage")
        << getNameSourceRange(Decl) << createFixItHint(Decl);
  }

  Check->diag(FirstDecl->getLocation(),
              "internal linkage specified on declaration here",
              DiagnosticIDs::Note)
      << getNameSourceRange(FirstDecl);
}

} // namespace

void Rule8p8Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  // clang-format off
  Finder->addMatcher(
    declaratorDecl(anyOf(
      functionDecl(
        isFirstDecl(),
        isSimpleFunction(),
        isStaticStorageClass(),
        hasDifferentDeclWithNonStaticStorageClass()
      ).bind("fn"),
      varDecl(
        isFirstDecl(),
        isStaticStorageClass(),
        hasDifferentDeclWithNonStaticStorageClass()
      ).bind("var")
    )),
    this);

  // clang-format on
}

void Rule8p8Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  if (const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("fn")) {
    dispatchDecl(this, Decl);
    return;
  }

  if (const auto *Decl = Result.Nodes.getNodeAs<VarDecl>("var")) {
    dispatchDecl(this, Decl);
    return;
  }
}

} // namespace clang::tidy::misrac
