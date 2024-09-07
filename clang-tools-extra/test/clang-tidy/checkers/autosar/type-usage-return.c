// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Type.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"


Std_ReturnType Swc_Return01(uint32 Value)
{
    Dem_EventStatusType EventStatus = DEM_EVENT_STATUS_PREPASSED;

    if (Value & 0x01) return E_OK;

    // CHECK-MESSAGES: [[@LINE+1]]:29: warning: mismatching types 'Std_ReturnType' and 'Dem_EventStatusType' between function declaration and return statement [autosar-type-usage]
    if (Value != 0u) return EventStatus;

    // CHECK-MESSAGES: [[@LINE+1]]:30: warning: mismatching types 'Std_ReturnType' and 'NvM_RequestResultType' between function declaration and return statement [autosar-type-usage]
    if (Value & 0x07) return NVM_REQ_PENDING;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types 'Std_ReturnType' and 'unsigned int' between function declaration and return statement [autosar-type-usage]
    return 0u;
}

boolean Swc_Return02(NvM_RequestResultType Value)
{
    return (Value == NVM_REQ_OK);
}

boolean Swc_Return03(NvM_RequestResultType Value)
{
    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: invalid explicit cast involving type 'boolean' [autosar-type-usage]
    return (boolean) !(Value == NVM_REQ_OK);
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"


