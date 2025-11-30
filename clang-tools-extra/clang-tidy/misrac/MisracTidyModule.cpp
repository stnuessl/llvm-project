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
#include "Rule10p2Check.h"
#include "Rule10p3Check.h"
#include "Rule10p4Check.h"
#include "Rule10p5Check.h"
#include "Rule10p6Check.h"
#include "Rule10p7Check.h"
#include "Rule10p8Check.h"
#include "Rule12p3Check.h"
#include "Rule14p4Check.h"
#include "Rule15p5Check.h"
#include "Rule15p6Check.h"
#include "Rule15p7Check.h"
#include "Rule16p4Check.h"
#include "Rule16p7Check.h"
#include "Rule18p5Check.h"
#include "Rule18p7Check.h"
#include "Rule18p8Check.h"
#include "Rule19p2Check.h"

namespace clang::tidy {
namespace misrac {

class MisracModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<Rule10p1Check>(
        "misrac-rule-10p1");
    CheckFactories.registerCheck<Rule10p2Check>(
        "misrac-rule-10p2");
    CheckFactories.registerCheck<Rule10p3Check>(
        "misrac-rule-10p3");
    CheckFactories.registerCheck<Rule10p4Check>(
        "misrac-rule-10p4");
    CheckFactories.registerCheck<Rule10p5Check>(
        "misrac-rule-10p5");
    CheckFactories.registerCheck<Rule10p6Check>(
        "misrac-rule-10p6");
    CheckFactories.registerCheck<Rule10p7Check>(
        "misrac-rule-10p7");
    CheckFactories.registerCheck<Rule10p8Check>(
        "misrac-rule-10p8");
    CheckFactories.registerCheck<Rule12p3Check>(
        "misrac-rule-12p3");
    CheckFactories.registerCheck<Rule14p4Check>(
        "misrac-rule-14p4");
    CheckFactories.registerCheck<Rule15p5Check>(
        "misrac-rule-15p5");
    CheckFactories.registerCheck<Rule15p6Check>(
        "misrac-rule-15p6");
    CheckFactories.registerCheck<Rule15p7Check>(
        "misrac-rule-15p7");
    CheckFactories.registerCheck<Rule16p4Check>(
        "misrac-rule-16p4");
    CheckFactories.registerCheck<Rule16p7Check>(
        "misrac-rule-16p7");
    CheckFactories.registerCheck<Rule18p5Check>(
        "misrac-rule-18p5");
    CheckFactories.registerCheck<Rule18p7Check>(
        "misrac-rule-18p7");
    CheckFactories.registerCheck<Rule18p8Check>(
        "misrac-rule-18p8");
    CheckFactories.registerCheck<Rule19p2Check>(
        "misrac-rule-19p2");
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

