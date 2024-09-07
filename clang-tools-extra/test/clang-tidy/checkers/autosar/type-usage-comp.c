// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Type.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Relational01(void)
{
    Dem_EventStatusType Status = DEM_EVENT_STATUS_PASSED;

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: invalid use of type 'Dem_EventStatusType' in relational operation [autosar-type-usage]
    if (Status < DEM_EVENT_STATUS_FAILED) {}

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: invalid use of type 'Dem_EventStatusType' in relational operation [autosar-type-usage]
    if (Status > DEM_EVENT_STATUS_FAILED) {}

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: invalid use of type 'Dem_EventStatusType' in relational operation [autosar-type-usage]
    if (Status <= DEM_EVENT_STATUS_FAILED) {}

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: invalid use of type 'Dem_EventStatusType' in relational operation [autosar-type-usage]
    if (Status >= DEM_EVENT_STATUS_FAILED) {}

    // CHECK-MESSAGES: [[@LINE+1]]:11: warning: invalid use of type 'Dem_EventStatusType' in relational operation [autosar-type-usage]
    if (0 < Status) {}

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: invalid use of type 'Dem_EventStatusType' in relational operation [autosar-type-usage]
    if (Status > E_OK) {}
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

