//===------- EmbeddedTidyModule.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "MissingCompoundCheck.h"
#include "MissingElseCheck.h"
#include "NoEarlyReturnCheck.h"
#include "NoUnionCheck.h"

namespace clang::tidy {
namespace embedded {

class EmbeddedModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<MissingCompoundCheck>(
        "embedded-missing-compound");
    CheckFactories.registerCheck<MissingElseCheck>(
        "embedded-missing-else");
    CheckFactories.registerCheck<NoEarlyReturnCheck>(
        "embedded-no-early-return");
    CheckFactories.registerCheck<NoUnionCheck>(
        "embedded-no-union");
  }
};

// Register the EmbeddedModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<EmbeddedModule> X("embedded-module",
                                                      "Add Embedded checks.");

} // namespace embedded

// This anchor is used to force the linker to link in the generated object file
// and thus register the EmbeddedModule.
volatile int EmbeddedModuleAnchorSource = 0;

} // namespace clang::tidy
