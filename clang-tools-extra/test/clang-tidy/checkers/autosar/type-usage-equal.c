// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Type.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Equality01(void)
{
    NvM_RequestResultType RequestResult = NVM_REQ_NOT_OK;
    boolean Flag = FALSE;
    uint8 Data = 0u;

    if (Flag) {}

    if (Data != 0 || Flag) {}

    if (RequestResult == NVM_REQ_OK) {}

    if (RequestResult == NVM_REQ_OK || RequestResult == NVM_REQ_NOT_OK) {}

    if (Data != 0 || (RequestResult == NVM_REQ_OK && Flag != FALSE)) {}

    // CHECK-MESSAGES: [[@LINE+1]]:18: warning: mismatching types 'Std_ReturnType' and 'NvM_RequestResultType' in equality operation [autosar-type-usage]
    if (E_NOT_OK == RequestResult) {}

    // CHECK-MESSAGES: [[@LINE+1]]:23: warning: mismatching types 'NvM_RequestResultType' and 'unsigned int' in equality operation [autosar-type-usage]
    if (RequestResult != 0u) {}

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: mismatching types 'Std_ReturnType' and 'uint8' in equality operation [autosar-type-usage]
    if (E_OK != Data) {}

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: mismatching types 'Std_ReturnType' and 'NvM_RequestResultType' in equality operation [autosar-type-usage]
    if (E_OK == NVM_REQ_OK) {}

    // CHECK-MESSAGES: [[@LINE+1]]:23: warning: mismatching types 'NvM_RequestResultType' and 'boolean' in equality operation [autosar-type-usage]
    if (RequestResult != Flag) {}
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"
