// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Type.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Switch01(NvM_RequestResultType Result)
{
    switch (Result)
    {
    case NVM_REQ_OK:
        break;
    case NVM_REQ_PENDING:
        break;
    case NVM_REQ_NOT_OK:
        break;
    default:
        break;
    }

    switch (Result)
    {
    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: mismatching types 'NvM_RequestResultType' and 'int' between switch condition and case statement [autosar-type-usage]
    case 0:
        break;
    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: mismatching types 'NvM_RequestResultType' and 'int' between switch condition and case statement [autosar-type-usage]
    case 1:
        break;
    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: mismatching types 'NvM_RequestResultType' and 'int' between switch condition and case statement [autosar-type-usage]
    case 2:
        break;
    default:
        break;
    }

    switch (Result)
    {
    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: mismatching types 'NvM_RequestResultType' and 'Dem_EventStatusType' between switch condition and case statement [autosar-type-usage]
    case DEM_EVENT_STATUS_PASSED:
        break;
    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: mismatching types 'NvM_RequestResultType' and 'Dem_EventStatusType' between switch condition and case statement [autosar-type-usage]
    case DEM_EVENT_STATUS_FAILED:
        break;
    default:
        break;
    }
}

boolean Swc_Switch02(NvM_RequestResultType Result)
{
    boolean Ok = TRUE;

    switch (Result) {
    case (NVM_REQ_OK):
    case (NVM_REQ_PENDING):
    case (NVM_REQ_NOT_OK):
        break;
    default:
        // CHECK-MESSAGES: [[@LINE+1]]:12: warning: invalid use of type 'boolean' in bit operation [autosar-type-usage]
        Ok |= FALSE;
        break;
    }

    switch (Ok) {
    case FALSE:
    case TRUE:
    default:
        break;
    }

    switch (Result == NVM_REQ_OK) {
    case FALSE:
    case TRUE:
    default:
        break;
    }

    return Ok;
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"
