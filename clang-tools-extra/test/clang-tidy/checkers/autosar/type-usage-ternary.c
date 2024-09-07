// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Swc.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Ternary01(boolean Flag)
{
    Dem_EventStatusType Status = (Flag) ? DEM_EVENT_STATUS_PASSED : DEM_EVENT_STATUS_FAILED;
    uint32 Data = 0u;

    (void) Dem_SetEventStatus((Dem_EventIdType) 0u, (Flag) ? DEM_EVENT_STATUS_PASSED : DEM_EVENT_STATUS_FAILED);

    // CHECK-MESSAGES: [[@LINE+1]]:47: warning: mismatching types 'Dem_EventStatusType' and 'Std_ReturnType' in conditional operation [autosar-type-usage]
    Status = (Flag) ? DEM_EVENT_STATUS_PASSED : E_OK;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: mismatching types 'Dem_EventStatusType' and 'Std_ReturnType' in assignment operation [autosar-type-usage]
    Status = (Flag) ? E_NOT_OK : E_OK;

    // CHECK-MESSAGES: [[@LINE+2]]:12: warning: mismatching types 'Dem_EventStatusType' and 'Std_ReturnType' in assignment operation [autosar-type-usage]
    // CHECK-MESSAGES: [[@LINE+1]]:28: warning: mismatching types 'Std_ReturnType' and 'unsigned int' in conditional operation [autosar-type-usage]
    Status = (Flag) ? E_OK : 1u;

    Data = (Status == DEM_EVENT_STATUS_FAILED) ? 0u : 1u;

    // CHECK-MESSAGES: [[@LINE+1]]:18: warning: invalid use of type 'Std_ReturnType' in arithmetic operation [autosar-type-usage]
    Data = (E_OK + E_NOT_OK) ? 0u : 1u;
}


#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

