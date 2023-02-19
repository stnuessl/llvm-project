// RUN: %check_clang_tidy %s autosar-memory-mapping %t -- -- -I%S/Inputs

#include "Swc.h"

#define Swc_START_SEC_VAR_CLEARED_8
#include "Swc_MemMap.h"

static uint8 Swc_Var01;

#define Swc_STOP_SEC_VAR_CLEARED_8
#include "Swc_MemMap.h"

#define Swc_START_SEC_CODE
#include "memory-mapping-filebreak.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

#include "memory-mapping-filebreak.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: memory mapping section started by 'Swc_START_SEC_CODE' not stopped [autosar-memory-mapping]

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Function01(void);

#include "memory-mapping-filebreak.h"
// CHECK-MESSAGES: [[@LINE-1]]:1: warning: memory mapping section started by 'Swc_START_SEC_CODE' not stopped [autosar-memory-mapping]

#define Swc_START_SEC_CONST_UNSPECIFIED
#include "Swc_MemMap.h"

const uint16 Swc_Var02 = 0;

#define Swc_STOP_SEC_CONST_UNSPECIFIED
#include "memory-mapping-filebreak.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Function02(void);

#define Swc_STOP_SEC_CODE 
#include "Swc_MemMap.h"

