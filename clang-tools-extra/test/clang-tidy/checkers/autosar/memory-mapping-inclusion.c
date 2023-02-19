// RUN: %check_clang_tidy %s autosar-memory-mapping %t -- -- -I%S/Inputs
// RUN: %check_clang_tidy -check-suffix=MOD %s autosar-memory-mapping %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             autosar-memory-mapping.AllowStaticLocalVariables: true \
// RUN:         } \
// RUN:     }' \
// RUN:     -- \
// RUN:     -I%S/Inputs

#include "Std_Types.h"

#define Swc_START_SEC_CONST_32
#include "Swc_MemMap.h"

static const uint32 Swc_Var01 = 0;

#define Swc_STOP_SEC_CONST_32
#include "Swc_MemMap.h"

#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: missing keyword at start of memory mapping section [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-2]]:1: warning: missing keyword at start of memory mapping section [autosar-memory-mapping]


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

uint32 Swc_Function01(void)
{
#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: invalid inclusion of memory mapping header inside the body of a function [autosar-memory-mapping]

#define Swc_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: invalid inclusion of memory mapping header inside the body of a function [autosar-memory-mapping]
    static uint32 Counter;
#define Swc_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: invalid inclusion of memory mapping header inside the body of a function [autosar-memory-mapping]

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: invalid inclusion of memory mapping header inside the body of a function [autosar-memory-mapping]

    return Counter++;
}
#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

#define Swc_START_SEC_VAR_INIT_UNSPECIFIED
#include "Swc_MemMap.h"

static uint32 Swc_Var02 = 0;

#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: missing keyword at end of memory mapping section [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-2]]:1: warning: missing keyword at end of memory mapping section [autosar-memory-mapping]

#define Swc_START_SEC_VAR_CLEARED_BOOLEAN
#include "Swc_MemMap.h"

static boolean Swc_Var03;

#define Swc_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Generic_MemMap.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: file name mismatch with previous inclusion [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-2]]:1: warning: file name mismatch with previous inclusion [autosar-memory-mapping]


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Function02(void) {}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

