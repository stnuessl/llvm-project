//===--- Rule8p10Check.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P10CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P10CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

namespace {

AST_MATCHER(FunctionDecl, isFunctionDeclKind) {
  return (&Node)->getKind() == Decl::Kind::Function;
}

} // namespace

/// Detects issues in accordance with MISRA C rule 8p10:
/// "An inline function shall be declared with the static storage class".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-8p10.html
class Rule8p10Check : public MisracBaseCheck {
public:
  Rule8p10Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;

    /* clang-format off */
    Finder->addMatcher(
        functionDecl(
          isInline(),
          // Avoid C++ methods as they are often contained within class
          // definitions and the 'static' qualifier has a different meaning
          // there.
          isFunctionDeclKind(),
          unless(isStaticStorageClass())
        ).bind("decl"),
        this);
    /* clang-format on */
  }

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Decl = Result.Nodes.getNodeAs<FunctionDecl>("decl");

    diag(Decl->getLocation(),
         "'static' storage classifier required in inline function declaration")
        << SourceRange(Decl->getBeginLoc(), Decl->getTypeSpecEndLoc())
        << FixItHint::CreateInsertion(Decl->getBeginLoc(), "static ");
  }
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P10CHECK_H
