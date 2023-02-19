// RUN: %check_clang_tidy %s autosar-memory-mapping %t -- -- -I%S/Inputs
// RUN: %check_clang_tidy -check-suffix=MOD %s autosar-memory-mapping %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             autosar-memory-mapping.Align32: 32;32BIT, \
// RUN:             autosar-memory-mapping.AllowEmptySections: true, \
// RUN:             autosar-memory-mapping.AllowRedundantSectionBreaks: true, \
// RUN:             autosar-memory-mapping.StrictAlignmentSpecifications: true \
// RUN:         } \
// RUN:     }' \
// RUN:     -- \
// RUN:     -I%S/Inputs

#include "Swc.h"

#define Swc_START_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"

static sint32 Swc_Var01;

#define Swc_STOP_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"

#define Swc_STOP_SEC_CONST_UNSPECIFIED
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: no memory mapping section started [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-3]]:9: warning: no memory mapping section started [autosar-memory-mapping]


#define Swc_START_SEC_VAR_INIT_64
// CHECK-MESSAGES: [[@LINE-1]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping] 
// CHECK-MESSAGES-MOD: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping] 

static sint64 Swc_Var02 = 0;

#define Swc_STOP_SEC_VAR_INIT_64
#include "Swc_MemMap.h"


#define Swc_START_SEC_VAR_CLEARED_BOOLEAN
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: unallowed empty memory mapping section [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE+2]]:9: note: empty memory section ends here

#define Swc_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Swc_MemMap.h"

#define Swc_START_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: memory mapping section started by 'Swc_START_SEC_VAR_CLEARED_32' not stopped [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE+4]]:9: note: next section starts here
// CHECK-MESSAGES-MOD: [[@LINE-4]]:9: warning: memory mapping section started by 'Swc_START_SEC_VAR_CLEARED_32' not stopped [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE+2]]:9: note: next section starts here

#define Swc_START_SEC_CONST_16
#include "Swc_MemMap.h"

const static sint16 Swc_Var03 = 0;

#define Swc_STOP_SEC_CONST_16
// CHECK-MESSAGES: [[@LINE-1]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping] 
// CHECK-MESSAGES-MOD: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping] 

#define Swc_START_SEC_CONST
#include "Swc_MemMap.h"
// CHECK-MESSAGES-MOD: [[@LINE-2]]:9: warning: missing or invalid alignment specification in keyword 'Swc_START_SEC_CONST' [autosar-memory-mapping]

const static uint32 Swc_Var04 = 0;

#define Swc_STOP_SEC_CONST
#include "Swc_MemMap.h"
// CHECK-MESSAGES-MOD: [[@LINE-2]]:9: warning: missing or invalid alignment specification in keyword 'Swc_STOP_SEC_CONST' [autosar-memory-mapping]

#define Swc_START_SEC_CONST_32BIT
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing or invalid alignment specification in keyword 'Swc_START_SEC_CONST_32BIT' [autosar-memory-mapping]

const static uint32 Swc_Var05 = 0;

#define Swc_STOP_SEC_CONST_32BIT
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing or invalid alignment specification in keyword 'Swc_STOP_SEC_CONST_32BIT' [autosar-memory-mapping]

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

static void Swc_Function01(void) {}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

