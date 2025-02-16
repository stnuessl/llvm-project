//===--- MissingElseCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingElseCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::embedded {

void MissingElseCheck::registerMatchers(MatchFinder *Finder) {
  // clang-format off

  Finder->addMatcher(
    ifStmt(
      hasElse(ifStmt(unless(hasElse(anything()))))
    ).bind("Stmt"),
    this);

  // clang-format on
}

void MissingElseCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *SM = Result.SourceManager;
  const auto *Stmt = Result.Nodes.getNodeAs<IfStmt>("Stmt");
  const auto *ElseStmt = Stmt->getElse();

  auto BeginLoc = Stmt->getElseLoc();
  auto [FileID, Offset] = SM->getDecomposedSpellingLoc(BeginLoc);
  auto Column = SM->getColumnNumber(FileID, Offset) - 1;

  auto EndLoc = ElseStmt->getEndLoc();
  auto Line = SM->getSpellingLineNumber(EndLoc);

  auto Loc = SM->translateLineCol(FileID, Line + 1, /* Column */ 1);

  llvm::SmallString<32> Code;

  switch (Style) {
  case IndentationStyle::NONE:
  case IndentationStyle::UNKNOWN:
  default:
    Code.append("else { /* Nothing to do */ }");
    break;
  case IndentationStyle::ALLMAN:
    Code.append(Column, ' ');
    Code.append("else\n");
    Code.append(Column, ' ');
    Code.append("{\n");
    Code.append(Column + Indentation, ' ');
    Code.append("/* Nothing to do */\n");
    Code.append(Column, ' ');
    Code.append("}\n");
    break;
  case IndentationStyle::KERNIGHAN_RICHIE:
    Code.append(Column, ' ');
    Code.append("else {\n");
    Code.append(Column + Indentation, ' ');
    Code.append("/* Nothing to do */\n");
    Code.append(Column, ' ');
    Code.append("}\n");
    break;
  }

  diag(BeginLoc, "'else if' must be terminated with an 'else' statement")
      << FixItHint::CreateInsertion(Loc, Code);
}

MissingElseCheck::IndentationStyle
MissingElseCheck::getIndentationStyle(StringRef Key) {

  auto Value = llvm::StringSwitch<IndentationStyle>(Key)
                   .CaseLower("None", IndentationStyle::NONE)
                   .CaseLower("Allman", IndentationStyle::ALLMAN)
                   .CaseLower("K&R", IndentationStyle::KERNIGHAN_RICHIE)
                   .Default(IndentationStyle::UNKNOWN);

  if (Value == IndentationStyle::UNKNOWN)
    configurationDiag("option 'IndentationStyle': invalid value '%0'") << Key;

  return Value;
}

} // namespace clang::tidy::embedded
