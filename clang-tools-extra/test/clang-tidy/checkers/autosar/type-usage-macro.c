// RUN: %check_clang_tidy %s autosar-type-usage %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             autosar-type-usage.Option.Add: \
// RUN:                 NvM_RequestResultType; \
// RUN:                 Swc_EventStatusType, \
// RUN:             autosar-type-usage.NvM_RequestResultType: \
// RUN:                 SWC_NVM_REQ_OK; \
// RUN:                 SWC_NVM_REQ_NOT_OK, \
// RUN:             autosar-type-usage.Swc_EventStatusType: \
// RUN:                 SWC_DEM_EVENT_STATUS_PASSED; \
// RUN:                 SWC_DEM_EVENT_STATUS_FAILED, \
// RUN:         } \
// RUN:     }' \
// RUN:     -- \
// RUN:     -I%S/Inputs

#include "Rte_Type.h"

#define SWC_NVM_REQ_OK (NVM_REQ_OK)
#define SWC_NVM_REQ_NOT_OK (NVM_REQ_NOT_OK)

#define SWC_DEM_EVENT_STATUS_PASSED (DEM_EVENT_STATUS_PASSED)
#define SWC_DEM_EVENT_STATUS_FAILED (DEM_EVENT_STATUS_FAILED)

typedef Dem_EventStatusType Swc_EventStatusType;

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Macro01(void)
{
    NvM_RequestResultType NvmResult = NVM_REQ_OK;
    Swc_EventStatusType Status = SWC_DEM_EVENT_STATUS_PASSED;

    NvmResult = SWC_NVM_REQ_OK;
    NvmResult = SWC_NVM_REQ_NOT_OK;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types "Swc_EventStatusType" and "Dem_EventStatusType" in assignment operation [autosar-type-usage]
    Status = DEM_EVENT_STATUS_PASSED;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types "Swc_EventStatusType" and "NvM_RequestResultType" in assignment operation [autosar-type-usage]
    Status = SWC_NVM_REQ_NOT_OK;
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"
