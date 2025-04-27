//===--- Rule18p7Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P7CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P7CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 18.7:
/// "Flexible array members shall not be declared".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-18p7.html
class Rule18p7Check : public MisracBaseCheck {
public:
  Rule18p7Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    Finder->addMatcher(fieldDecl(hasType(incompleteArrayType())).bind("decl"),
                       this);
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Decl = Result.Nodes.getNodeAs<FieldDecl>("decl");

    diag(Decl->getLocation(), "flexible array member used")
        << SourceRange(Decl->getLocation(), Decl->getEndLoc());
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE18P7CHECK_H
