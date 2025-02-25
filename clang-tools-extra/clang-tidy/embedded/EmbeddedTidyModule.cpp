//===------- EmbeddedTidyModule.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "DefaultCasePositionCheck.h"
#include "MissingCompoundCheck.h"
#include "MissingElseCheck.h"
#include "MissingStaticCheck.h"
#include "NoCommaOperatorCheck.h"
#include "NoEarlyReturnCheck.h"
#include "NoFlexibleArrayMemberCheck.h"
#include "NoPartialArrayInitCheck.h"
#include "NoStdargFeaturesCheck.h"
#include "NoUnionCheck.h"
#include "NoVariableLengthArrayCheck.h"

namespace clang::tidy {
namespace embedded {

class EmbeddedModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<DefaultCasePositionCheck>(
        "embedded-default-case-position");
    CheckFactories.registerCheck<MissingCompoundCheck>(
        "embedded-missing-compound");
    CheckFactories.registerCheck<MissingElseCheck>(
        "embedded-missing-else");
    CheckFactories.registerCheck<MissingStaticCheck>(
        "embedded-missing-static");
    CheckFactories.registerCheck<NoCommaOperatorCheck>(
        "embedded-no-comma-operator");
    CheckFactories.registerCheck<NoEarlyReturnCheck>(
        "embedded-no-early-return");
    CheckFactories.registerCheck<NoFlexibleArrayMemberCheck>(
        "embedded-no-flexible-array-member");
    CheckFactories.registerCheck<NoPartialArrayInitCheck>(
        "embedded-no-partial-array-init");
    CheckFactories.registerCheck<NoStdargFeaturesCheck>(
        "embedded-no-stdarg-features");
    CheckFactories.registerCheck<NoUnionCheck>(
        "embedded-no-union");
    CheckFactories.registerCheck<NoVariableLengthArrayCheck>(
        "embedded-no-variable-length-array");
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
