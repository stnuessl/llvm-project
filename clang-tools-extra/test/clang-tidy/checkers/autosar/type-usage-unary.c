// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Std_Types.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Unary01(boolean Flag)
{
    boolean IsValid = FALSE;
    Std_ReturnType RetVal = E_OK;

    IsValid = !IsValid;
    IsValid = !FALSE;
    IsValid = !TRUE;

    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: invalid use of type 'boolean' in arithmetic operation [autosar-type-usage]
    IsValid = -TRUE;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: invalid use of type 'boolean' in arithmetic operation [autosar-type-usage]
    ++IsValid;

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: invalid use of type 'Std_ReturnType' in logical operation [autosar-type-usage]
    RetVal = !RetVal;
}


#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

