//===--- Rule8p12Check.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8p12Check.h"

namespace clang::tidy::misrac {

void Rule8p12Check::addMatchers(ast_matchers::MatchFinder *Finder) {
  using namespace clang::ast_matchers;

  Finder->addMatcher(enumDecl().bind("decl"), this);
}

void Rule8p12Check::dispatch(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  const auto *Decl = Result.Nodes.getNodeAs<EnumDecl>("decl");

  dispatch(Decl);
}

void Rule8p12Check::dispatch(const EnumDecl *Decl) {
  Map.clear();

  for (const auto *ECD : Decl->enumerators()) {
    if (ECD->getInitExpr())
      (void)Map.insert({ECD->getValue(), ECD});
  }

  for (const auto *ECD : Decl->enumerators()) {
    if (ECD->getInitExpr())
      continue;

    auto [It, Ok] = Map.insert({ECD->getValue(), ECD});
    if (Ok || It == Map.end())
      continue;

    SmallString<32> Buffer;
    auto OS = llvm::raw_svector_ostream(Buffer);
    OS << ECD->getName() << " = " << ECD->getValue();

    auto Range = ECD->getSourceRange();

    diag(ECD->getLocation(), "non-unique implicitly defined enumeration value")
        << Range << FixItHint::CreateReplacement(Range, Buffer);

    diag(It->second->getLocation(),
         "'%0' defined with the same value as '%1' here", DiagnosticIDs::Note)
        << It->second->getSourceRange() << It->second->getName()
        << ECD->getName();
  }
}

} // namespace clang::tidy::misrac
