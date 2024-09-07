// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Swc.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

Std_ReturnType Swc_Arithmetic01(void)
{
    NvM_RequestResultType NvmStatus = NVM_REQ_NOT_OK;
    Std_ReturnType RetVal = E_OK;
    uint32 Value;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: invalid use of type 'Std_ReturnType' in bit operation [autosar-type-usage]
    RetVal |= Dem_SetEventStatus((Dem_EventIdType) 0u, DEM_EVENT_STATUS_PASSED);
    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: invalid use of type 'Std_ReturnType' in bit operation [autosar-type-usage]
    RetVal |= NvM_GetErrorStatus((NvM_BlockIdType) 0u, &NvmStatus);
    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: invalid use of type 'Std_ReturnType' in bit operation [autosar-type-usage]
    RetVal |= Rte_Write_OutPort_DataElement01(TRUE);

    // CHECK-MESSAGES: [[@LINE+1]]:27: warning: invalid use of type 'NvM_RequestResultType' in bit operation [autosar-type-usage]
    NvmStatus = NvmStatus & NVM_REQ_PENDING;

    // CHECK-MESSAGES: [[@LINE+1]]:27: warning: invalid use of type 'NvM_RequestResultType' in arithmetic operation [autosar-type-usage]
    NvmStatus = NvmStatus - 1u;

    // CHECK-MESSAGES: [[@LINE+2]]:11: warning: mismatching types 'uint32' and 'Std_ReturnType' in assignment operation [autosar-type-usage]
    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: invalid use of type 'Std_ReturnType' in arithmetic operation [autosar-type-usage]
    Value = RetVal + E_NOT_OK;

    // CHECK-MESSAGES: [[@LINE+2]]:11: warning: mismatching types 'uint32' and 'NvM_RequestResultType' in assignment operation [autosar-type-usage]
    // CHECK-MESSAGES: [[@LINE+1]]:23: warning: invalid use of type 'NvM_RequestResultType' in bit operation [autosar-type-usage]
    Value = NvmStatus | 1u;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types 'Std_ReturnType' and 'unsigned int' in assignment operation [autosar-type-usage]
    RetVal = 1u + 1u;

    return RetVal;
}


#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

