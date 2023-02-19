// RUN: %check_clang_tidy %s autosar-memory-mapping %t -- -- -I%S/Inputs
// RUN: %check_clang_tidy -check-suffix=MOD %s autosar-memory-mapping %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             autosar-memory-mapping.AllowLegacyInitializations: true \
// RUN:         } \
// RUN:     }' \
// RUN:     -- \
// RUN:     -I%S/Inputs

//
// This file contains all checks related to 
// CheckDetail::operator()(const VarDeclProxy &Proxy)
//

#include "Swc.h"

#define Swc_START_SEC_VAR_CLEARED_BOOLEAN
#include "Swc_MemMap.h"

static boolean Swc_Var01;

#define Swc_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Swc_MemMap.h"

#define Swc_START_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"

static uint32 Swc_Var02;

#define Swc_STOP_SEC_VAR_CLEARED_32
// CHECK-MESSAGES: [[@LINE-1]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]


static uint16 Swc_Var03;
// CHECK-MESSAGES: [[@LINE-1]]:15: warning: declaration not memory mapped [autosar-memory-mapping] 
// CHECK-MESSAGES-MOD: [[@LINE-2]]:15: warning: declaration not memory mapped [autosar-memory-mapping] 


#define Swc_START_SEC_VAR_INIT_8
// CHECK-MESSAGES: [[@LINE-1]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]

static uint8 Swc_Var04 = 0;
static uint8 Swc_Var05 = 0;

#define Swc_STOP_SEC_VAR_INIT_8
#include "Swc_MemMap.h"


#define Swc_START_SEC_VAR_INIT_32
#include "Swc_MemMap.h"

uint32 Swc_GlobalData = 0;
// CHECK-MESSAGES: [[@LINE-1]]:8: warning: entity mapped with different keywords: 'Swc_START_SEC_VAR_INIT_UNSPECIFIED' and 'Swc_START_SEC_VAR_INIT_32' [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-2]]:8: warning: entity mapped with different keywords: 'Swc_START_SEC_VAR_INIT_UNSPECIFIED' and 'Swc_START_SEC_VAR_INIT_32' [autosar-memory-mapping]

#define Swc_STOP_SEC_VAR_INIT_32
#include "Swc_MemMap.h"


#define Swc_START_SEC_CONST_PTR
#include "Swc_MemMap.h"

static uint8 *const Swc_Var06;
static const sint16 *const Swc_Var07;
static const uint32 *Swc_Var08;
// CHECK-MESSAGES: [[@LINE-1]]:22: warning: invalid mapping of non-const variable 'Swc_Var08' to 'CONST' section [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-7]]:9: note: section started with 'Swc_START_SEC_CONST_PTR' here
// CHECK-MESSAGES-MOD: [[@LINE-3]]:22: warning: invalid mapping of non-const variable 'Swc_Var08' to 'CONST' section [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-9]]:9: note: section started with 'Swc_START_SEC_CONST_PTR' here

#define Swc_STOP_SEC_CONST_PTR
#include "Swc_MemMap.h"


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

static boolean Swc_Var09;
// CHECK-MESSAGES: [[@LINE-1]]:16: warning: invalid mapping of variable 'Swc_Var09' to 'CODE' section [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-5]]:9: note: section started with 'Swc_START_SEC_CODE' here
// CHECK-MESSAGES-MOD: [[@LINE-3]]:16: warning: invalid mapping of variable 'Swc_Var09' to 'CODE' section [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-7]]:9: note: section started with 'Swc_START_SEC_CODE' here

static boolean Swc_Var10;
// CHECK-MESSAGES: [[@LINE-1]]:16: warning: invalid mapping of variable 'Swc_Var10' to 'CODE' section [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-11]]:9: note: section started with 'Swc_START_SEC_CODE' here
// CHECK-MESSAGES-MOD: [[@LINE-3]]:16: warning: invalid mapping of variable 'Swc_Var10' to 'CODE' section [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-13]]:9: note: section started with 'Swc_START_SEC_CODE' here

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"


#define Swc_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"

extern uint16 Swc_Var11;
uint16 Swc_Var11 = 0;
// CHECK-MESSAGES: [[@LINE-1]]:20: warning: invalid explicit initialization of 'Swc_Var11' [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-6]]:9: note: 'Swc_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED' allows only implicit initializiations
// CHECK-MESSAGES-MOD: [[@LINE-3]]:20: warning: invalid explicit initialization of 'Swc_Var11' [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-8]]:9: note: 'Swc_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED' allows only implicit initializiations

#define Swc_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"


#define Swc_START_SEC_VAR_INIT_64
#include "Swc_MemMap.h"

extern uint64 Swc_Var12;
uint64 Swc_Var12;
uint64 Swc_Var12;
uint64 Swc_Var12;
// CHECK-MESSAGES: [[@LINE-1]]:8: warning: missing explicit initialization of 'Swc_Var12' [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-8]]:9: note: 'Swc_START_SEC_VAR_INIT_64' requires explicit initializations

#define Swc_STOP_SEC_VAR_INIT_64
#include "Swc_MemMap.h"


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

static void Swc_Function01(void) {}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"


