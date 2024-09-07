// RUN: %check_clang_tidy %s autosar-type-usage %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             autosar-type-usage.Option.Add: \
// RUN:                 bool; \
// RUN:                 NvM_RequestResultType; \
// RUN:                 Swc_EventStatusType, \
// RUN:             autosar-type-usage.Option.Remove: \
// RUN:                 Std_ReturnType; \
// RUN:                 ComM_ModeType, \
// RUN:             autosar-type-usage.bool: \
// RUN:                 false; \
// RUN:                 true, \
// RUN:             autosar-type-usage.NvM_RequestResultType: \
// RUN:                 SWC_NVM_REQ_OK; \
// RUN:                 SWC_NVM_REQ_NOT_OK, \
// RUN:             autosar-type-usage.Swc_EventStatusType: \
// RUN:                 SWC_DEM_EVENT_STATUS_FAILED; \
// RUN:                 SWC_DEM_EVENT_STATUS_PASSED, \
// RUN:         } \
// RUN:     }' \
// RUN:     -- \
// RUN:     -I%S/Inputs

#include "Rte_Type.h"

typedef _Bool bool;

#define false 0
#define true 1

// Additional values for known type NvM_RequestResultType
#define SWC_NVM_REQ_OK (NVM_REQ_OK)
#define SWC_NVM_REQ_NOT_OK (NVM_REQ_NOT_OK)

// Custom values for new type Swc_EventStatusType
#define SWC_DEM_EVENT_STATUS_PASSED (DEM_EVENT_STATUS_PASSED)
#define SWC_DEM_EVENT_STATUS_FAILED (DEM_EVENT_STATUS_FAILED)

typedef Dem_EventStatusType Swc_EventStatusType;


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Bool01(void)
{
    bool Value01 = false;

    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: a value of type 'boolean' cannot be used to initialize a variable of type 'bool' [autosar-type-usage]
    bool Value02 = FALSE;

    Value01 = true;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types 'bool' and 'boolean' in assignment operation [autosar-type-usage]
    Value02 = TRUE;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types 'bool' and 'int' in assignment operation [autosar-type-usage]
    Value02 = 0;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types 'bool' and 'int' in assignment operation [autosar-type-usage]
    Value02 = 1;
}

void Swc_Ignore01(void)
{
    Std_ReturnType RetVal = E_OK;

    // No warnings as the used 'types' are getting ignored by the configuration.
    RetVal = E_NOT_OK;
    RetVal = COMM_NO_COMMUNICATION;
    RetVal = 0u;
}

void Swc_MacroValues01(void)
{
    NvM_RequestResultType NvmResult = NVM_REQ_OK;
    Swc_EventStatusType Status = SWC_DEM_EVENT_STATUS_PASSED;

    NvmResult = SWC_NVM_REQ_OK;
    NvmResult = SWC_NVM_REQ_NOT_OK;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types 'Swc_EventStatusType' and 'Dem_EventStatusType' in assignment operation [autosar-type-usage]
    Status = DEM_EVENT_STATUS_PASSED;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types 'Swc_EventStatusType' and 'NvM_RequestResultType' in assignment operation [autosar-type-usage]
    Status = SWC_NVM_REQ_NOT_OK;
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

