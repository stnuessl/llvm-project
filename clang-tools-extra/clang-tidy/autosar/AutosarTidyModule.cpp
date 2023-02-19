//===------- AutosarTidyModule.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "MemoryMappingCheck.h"

namespace clang::tidy {
namespace autosar {

class AutosarModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<MemoryMappingCheck>("autosar-memory-mapping");
  }
};

// Register the AutosarModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<AutosarModule> X("autosar-module",
                                                     "Add Autosar checks.");

} // namespace autosar

// This anchor is used to force the linker to link in the generated object file
// and thus register the AutosarModule.
volatile int AutosarModuleAnchorSource = 0;

} // namespace clang::tidy
