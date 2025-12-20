//===--- Rule8p3Check.h - clang-tidy ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P3CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P3CHECK_H

#include "MisracBaseCheck.h"

namespace clang::tidy::misrac {

/// Detects issues in accordance with MISRA C rule 8.3:
/// "All declarations of an object or function shall use the same names and
/// type qualifiers".
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misrac/rule-8p3.html
class Rule8p3Check : public MisracBaseCheck {
public:
  Rule8p3Check(StringRef Name, ClangTidyContext *Context)
      : MisracBaseCheck(Name, Context) {}

  void addMatchers(ast_matchers::MatchFinder *Finder) override;

protected:
  void dispatch(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  FixItHint createTypeReplacement(SourceRange Range, QualType Type) {
    auto Buffer = SmallString<32>();
    auto OS = llvm::raw_svector_ostream(Buffer);
    auto Policy = PrintingPolicy(getLangOpts());

    Type.print(OS, Policy);

    // Ensure there is a space separating tokens after the replacment.
    // Example:     int *p --> int *const p
    if (Type->isPointerType() && Buffer.back() != '*')
      Buffer.push_back(' ');

    return FixItHint::CreateReplacement(Range, Buffer);
  }

  void dispatch(const VarDecl *Decl);
  void dispatch(const FunctionDecl *Decl);
  void dispatchReturnType(const FunctionDecl *Decl);
  void dispatchParameters(const FunctionDecl *Decl);

  void dispatch(const DeclaratorDecl *FirstDecl, const DeclaratorDecl *Decl);

  void emitWarning(SourceRange Range, QualType FirstType);

  void emitNote(const DeclaratorDecl *FirstDecl, const ValueDecl *Decl);
  void emitNoteReturnType(const FunctionDecl *FirstDecl,
                          const FunctionDecl *Decl);
};

} // namespace clang::tidy::misrac

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISRAC_RULE8P3CHECK_H
