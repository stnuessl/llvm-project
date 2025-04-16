//===------- MisracTidyModule.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "Rule10p1Check.h"

namespace clang::tidy {
namespace misrac {

class MisracModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<Rule10p1Check>(
        "misrac-rule-10p1");
  }
};

// Register the MisracModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<MisracModule> X("misrac-module",
                                                    "Add MISRA C checks.");

} // namespace misrac

// This anchor is used to force the linker to link in the generated object file
// and thus register the MisracModule.
volatile int MisracModuleAnchorSource = 0;

} // namespace clang::tidy

