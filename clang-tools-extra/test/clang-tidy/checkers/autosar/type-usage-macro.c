// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Swc.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

Std_ReturnType Swc_Macro01(uint32 Value)
{
    // Macro definitions hiding an explicit cast should _not_ result in an
    // explicit type cast warning
    Std_ReturnType Value1 = DCM_E_PENDING;
    Std_ReturnType Value2;

    Value2 = DCM_E_PENDING;

    if (Rte_Write_OutPort_DataElement02(FALSE) == RTE_E_UNCONNECTED) {}

    // CHECK-MESSAGES: [[@LINE+1]]:48: warning: mismatching types 'Std_ReturnType' and 'NvM_RequestResultType' in equality operation [autosar-type-usage]
    if (Rte_Write_OutPort_DataElement02(FALSE) == NVM_REQ_PENDING) {}

    return RTE_E_OK;
}


#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

