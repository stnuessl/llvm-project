//===--- Rule18p5Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P5CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P5CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 18.5:
/// "Declarations should contain no more than two levels of pointer nesting".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-18p5.html
class Rule18p5Check : public MisracBaseCheck {
public:
  Rule18p5Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context),
        MaxLevel(Options.get("MaxNestingLevel", 2)) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    auto Matcher = qualType(hasCanonicalType(pointerType()));

    for (unsigned Level = 0; Level < MaxLevel; ++Level)
      Matcher = qualType(hasCanonicalType(pointerType(pointee(Matcher))));

    // clang-format off
  Finder->addMatcher(
    namedDecl(
      mapAnyOf(varDecl, fieldDecl, typedefDecl).with(hasType(Matcher))
    ).bind("decl"),
    this);
    // clang-format on
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Decl = Result.Nodes.getNodeAs<NamedDecl>("decl");

    diag(Decl->getBeginLoc(),
         "declaration execeeds pointer nesting level of %0")
        << MaxLevel << Decl->getSourceRange();
  }

  void writeOptions(ClangTidyOptions::OptionMap &Opts) override {
    Options.store(Opts, "MaxNestingLevel", MaxLevel);
  }

private:
  unsigned MaxLevel;
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P5CHECK_H
