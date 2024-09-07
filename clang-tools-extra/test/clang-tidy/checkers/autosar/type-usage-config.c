// RUN: %check_clang_tidy %s autosar-type-usage %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             autosar-type-usage.Option.Add: bool, \
// RUN:             autosar-type-usage.Option.Remove: \
// RUN:                 Std_ReturnType; \
// RUN:                 NvM_RequestResultType, \
// RUN:             autosar-type-usage.bool: true;false \
// RUN:         } \
// RUN:     }' \
// RUN:     -- \
// RUN:     -I%S/Inputs

#include "Rte_Type.h"

typedef _Bool bool;

#define false 0
#define true 1

void Swc_Bool01(void)
{
    bool Value01 = false;

    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: a value of type "boolean" cannot be used to initialize a variable of type "bool" [autosar-type-usage]
    bool Value02 = FALSE;

    Value01 = true;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types "bool" and "boolean" in assignment operation [autosar-type-usage]
    Value02 = TRUE;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types "bool" and "int" in assignment operation [autosar-type-usage]
    Value02 = 0;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types "bool" and "int" in assignment operation [autosar-type-usage]
    Value02 = 1;
}

void Swc_Ignore01(void)
{
    Std_ReturnType RetVal = E_OK;

    RetVal = E_NOT_OK;
    RetVal = NVM_REQ_OK;
    RetVal = 0u;
}

