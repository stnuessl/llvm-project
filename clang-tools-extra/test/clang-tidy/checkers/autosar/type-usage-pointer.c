// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Type.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Pointer01(NvM_RequestResultType *Result)
{
    *Result = NVM_REQ_OK;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types 'NvM_RequestResultType' and 'unsigned int' in assignment operation [autosar-type-usage]
    *Result = 0u;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: mismatching types 'NvM_RequestResultType' and 'Std_ReturnType' in assignment operation [autosar-type-usage]
    *Result = E_OK;

    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: mismatching types 'NvM_RequestResultType' and 'unsigned int' in assignment operation [autosar-type-usage]
    Result[0] = 0u;

    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: mismatching types 'NvM_RequestResultType' and 'Std_ReturnType' in assignment operation [autosar-type-usage]
    Result[0] = E_OK;
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

