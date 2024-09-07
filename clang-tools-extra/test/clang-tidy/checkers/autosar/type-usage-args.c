// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Swc.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_FunctionArguments01(void)
{
    NvM_RequestResultType ReqResult = NVM_REQ_OK;

    (void) Rte_Write_OutPort_DataElement01(FALSE);
    (void) Rte_Write_OutPort_DataElement01(TRUE);
    (void) Rte_Write_OutPort_DataElement01(ReqResult != NVM_REQ_NOT_OK);

    // CHECK-MESSAGES: [[@LINE+1]]:44: warning: mismatching types "boolean" and "unsigned int" between function parameter and passed argument [autosar-type-usage]
    (void) Rte_Write_OutPort_DataElement01(1u);

    // CHECK-MESSAGES: [[@LINE+1]]:44: warning: mismatching types "boolean" and "Dem_EventStatusType" between function parameter and passed argument [autosar-type-usage]
    (void) Rte_Write_OutPort_DataElement01(DEM_EVENT_STATUS_PREPASSED);

    // CHECK-MESSAGES: [[@LINE+1]]:44: warning: mismatching types "boolean" and "NvM_RequestResultType" between function parameter and passed argument [autosar-type-usage]
    (void) Rte_Write_OutPort_DataElement01(ReqResult);

    // CHECK-MESSAGES: [[@LINE+1]]:53: warning: mismatching types "Dem_EventStatusType" and "Std_ReturnType" between function parameter and passed argument [autosar-type-usage]
    (void) Dem_SetEventStatus((Dem_EventIdType) 0u, E_OK);
}

void Swc_MacroArguments01(void)
{
    boolean Value01 = FALSE;
    Std_ReturnType Value02 = E_OK;

    (void) Rte_Write_OutPort_DataElement02(FALSE);
    (void) Rte_Write_OutPort_DataElement02(TRUE);
    (void) Rte_Write_OutPort_DataElement02(Value01);
    (void) Rte_Write_OutPort_DataElement02(Value01 != FALSE);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types "boolean" and "Std_ReturnType" in assignment operation [autosar-type-usage]
    (void) Rte_Write_OutPort_DataElement02(E_OK);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types "boolean" and "NvM_RequestResultType" in assignment operation [autosar-type-usage]
    (void) Rte_Write_OutPort_DataElement02(NVM_REQ_OK);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types "boolean" and "Std_ReturnType" in assignment operation [autosar-type-usage]
    (void) Rte_Write_OutPort_DataElement02(Value02);
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

