// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Type.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Assignment01(void)
{
    NvM_RequestResultType Status01, Status02;
    Std_ReturnType RetVal;
    boolean Ok1, Ok2;
    uint32 Value;

    Status01 = Status02 = NVM_REQ_OK;
    RetVal = E_OK;
    Ok1 = (Value != 0u);
    Ok2 = !(Value != 0u);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types "Std_ReturnType" and "unsigned int" in assignment operation [autosar-type-usage]
    RetVal = 0u;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types "Std_ReturnType" and "boolean" in assignment operation [autosar-type-usage]
    RetVal = FALSE;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: mismatching types "boolean" and "Std_ReturnType" in assignment operation [autosar-type-usage]
    Ok2 = (Std_ReturnType) (Value != 0u);

    // CHECK-MESSAGES: [[@LINE+1]]:11: warning: mismatching types "uint32" and "Std_ReturnType" in assignment operation [autosar-type-usage]
    Value = E_OK;
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"
