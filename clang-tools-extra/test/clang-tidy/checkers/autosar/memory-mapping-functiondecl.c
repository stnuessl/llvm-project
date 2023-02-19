// RUN: %check_clang_tidy %s autosar-memory-mapping %t -- -- -I%S/Inputs

//
// This file contains all checks related to 
// CheckDetail::operator()(const FunctionDeclProxy &Proxy)
//

#include "Swc.h"

#define Swc_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"

uint32 Swc_Var01;

#define Swc_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Function01(void);

#define Swc_STOP_SEC_CODE
// CHECK-MESSAGES: [[@LINE-1]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]


void Swc_Function02(void);
// CHECK-MESSAGES: [[@LINE-1]]:6: warning: declaration not memory mapped [autosar-memory-mapping]

static void Swc_Function03(void) {}
// CHECK-MESSAGES: [[@LINE-1]]:13: warning: declaration not memory mapped [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-2]]:35: warning: end of function body not memory mapped [autosar-memory-mapping]


#define Swc_START_SEC_VAR_INIT_UNSPECIFIED
#include "Swc_MemMap.h"

uint32 Swc_Var02 = 0;

#define Swc_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Swc_MemMap.h"


#define Swc_START_SEC_CODE
// CHECK-MESSAGES: [[@LINE-1]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]

static void Swc_Function04(void) {}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"


#define SwcA_START_SEC_CODE
#include "Swc_MemMap.h"

Std_ReturnType Swc_GetStatus(void)
// CHECK-MESSAGES: [[@LINE-1]]:16: warning: entity mapped with different keywords: 'Swc_START_SEC_CODE' and 'SwcA_START_SEC_CODE' [autosar-memory-mapping]
{
    return E_OK;
}

#define SwcA_STOP_SEC_CODE
#include "Swc_MemMap.h"


#define Swc_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"

static void Swc_Function05(void) {}
// CHECK-MESSAGES: [[@LINE-1]]:13: warning: invalid mapping of function 'Swc_Function05' to 'VAR' section [autosar-memory-mapping]

#define Swc_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"


#define Swc_START_SEC_CONST_32
#include "Swc_MemMap.h"

static void Swc_Function06(void) {}
// CHECK-MESSAGES: [[@LINE-1]]:13: warning: invalid mapping of function 'Swc_Function06' to 'CONST' section [autosar-memory-mapping]

#define Swc_STOP_SEC_CONST_32
#include "Swc_MemMap.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

static uint32 Swc_Function07(uint32 Val)
{
    uint32 Num = 0;

    while (Val != 0) {
        Val = Val & (Val - 1);
        ++Num;
    }

    return Num;
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

