// RUN: %check_clang_tidy %s autosar-memory-mapping %t -- -- -I%S/Inputs
// RUN: %check_clang_tidy -check-suffix=MOD %s autosar-memory-mapping %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             autosar-memory-mapping.MemMapInclusion: \
// RUN:                 "*/Inputs/Swc_MemMap.h;*/autosar/Inputs/*_MemMap_*.h", \
// RUN:             autosar-memory-mapping.PolicyCleared: \
// RUN:                 "CLEARED;ZERO_INIT;NO_INIT;POWER_ON_CLEARED", \
// RUN:             autosar-memory-mapping.PolicyInit: \
// RUN:                 "INIT;POWER_ON_INIT", \
// RUN:             autosar-memory-mapping.SectionConst: \
// RUN:                 "CONST;READ_ONLY", \
// RUN:             autosar-memory-mapping.KeywordStartAffix: START_SEC_, \
// RUN:             autosar-memory-mapping.KeywordStopAffix: STOP_SEC_, \
// RUN:             autosar-memory-mapping.Align8: "8;8BIT", \
// RUN:             autosar-memory-mapping.Align16: "16;16BIT", \
// RUN:             autosar-memory-mapping.Align32: "32;32BIT", \
// RUN:             autosar-memory-mapping.Align64: "64;64BIT" \
// RUN:         } \
// RUN:     }' \
// RUN:     -- \
// RUN:     -I%S/Inputs

#include "Swc.h"

#define Swc_START_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"

static uint32 Swc_Var01;

#define Swc_STOP_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"


#define Swc_START_SEC_VAR_INIT_8
#include "Swc_MemMap.h"

static uint8 Swc_Var02;
// CHECK-MESSAGES: [[@LINE-1]]:14: warning: missing explicit initialization of 'Swc_Var02' [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-5]]:9: note: 'Swc_START_SEC_VAR_INIT_8' requires explicit initializations
// CHECK-MESSAGES-MOD: [[@LINE-3]]:14: warning: missing explicit initialization of 'Swc_Var02' [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-7]]:9: note: 'Swc_START_SEC_VAR_INIT_8' requires explicit initializations

#define Swc_STOP_SEC_VAR_INIT_8
#include "Swc_MemMap.h"


#define Swc_START_SEC_READ_ONLY_UNSPECIFIED
#include "Swc_MemMap_Extended.h"

static const sint32 Swc_Var03 = 0;
// CHECK-MESSAGES: [[@LINE-1]]:21: warning: declaration not memory mapped [autosar-memory-mapping]

#define Swc_STOP_SEC_READ_ONLY_UNSPECIFIED
#include "Swc_MemMap_Extended.h"


#define Swc_START_SEC_VAR_POWER_ON_INIT_32
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing or invalid initialization policy in keyword 'Swc_START_SEC_VAR_POWER_ON_INIT_32' [autosar-memory-mapping]

static uint32 Swc_Var04 = 0;

#define Swc_STOP_SEC_VAR_POWER_ON_INIT_32
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing or invalid initialization policy in keyword 'Swc_STOP_SEC_VAR_POWER_ON_INIT_32' [autosar-memory-mapping]


#define Swc_START_SEC_VAR_CLEARED_64BIT
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing or invalid alignment specification in keyword 'Swc_START_SEC_VAR_CLEARED_64BIT' [autosar-memory-mapping]

static sint64 Swc_Var05;

#define Swc_STOP_SEC_VAR_CLEARED_64BIT
#include "Swc_MemMap.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing or invalid alignment specification in keyword 'Swc_STOP_SEC_VAR_CLEARED_64BIT' [autosar-memory-mapping]


#define Swc_START_SEC_CODE
#include "Swc_MemMap_Extended.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]

void Swc_Function01(void);
// CHECK-MESSAGES: [[@LINE-1]]:6: warning: declaration not memory mapped [autosar-memory-mapping]

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap_Extended.h"
// CHECK-MESSAGES: [[@LINE-2]]:9: warning: missing inclusion of memory mapping header [autosar-memory-mapping]
// CHECK-MESSAGES: [[@LINE-3]]:9: warning: no memory mapping section started [autosar-memory-mapping]


#define START_SEC_VAR_ZERO_INIT_32BIT
#include "Swc_MemMap_Extended.h"

static uint32 Swc_Var06 = 0;
// CHECK-MESSAGES: [[@LINE-1]]:15: warning: declaration not memory mapped [autosar-memory-mapping]
// CHECK-MESSAGES-MOD: [[@LINE-2]]:27: warning: invalid explicit initialization of 'Swc_Var06' [autosar-memory-mapping] 
// CHECK-MESSAGES-MOD: [[@LINE-6]]:9: note: 'START_SEC_VAR_ZERO_INIT_32BIT' allows only implicit initializiations

#define STOP_SEC_VAR_ZERO_INIT_32BIT
#include "Swc_MemMap_Extended.h"


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Function02(void) {}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

