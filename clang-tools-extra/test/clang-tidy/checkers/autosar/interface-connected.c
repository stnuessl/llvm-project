// RUN: %check_clang_tidy %s autosar-interface-connected %t -- -- -I%S/Inputs

#include "Rte_Type.h"

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: unconnected macro interface 'Rte_Call_UnkownFault_SetEventStatus' [autosar-interface-connected]
#define Rte_Call_UnkownFault_SetEventStatus(EventStatus) (RTE_E_UNCONNECTED)

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: unconnected macro interface 'Rte_Read_RP_PortA_DataElement1' [autosar-interface-connected]
#define Rte_Read_RP_PortA_DataElement1(Value) \
    ((*Value) = 0u, (Std_ReturnType) RTE_E_UNCONNECTED)

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: unconnected macro interface 'Rte_Write_PP_PortB_DataElement1' [autosar-interface-connected]
#define Rte_Write_PP_PortB_DataElement1(Value) ((void) Value, RTE_E_UNCONNECTED)

#define Rte_Write_PP_PortB_DataElement2(Value) ((void) Value, RTE_E_OK)

#define Swc_START_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"

static uint32 Swc_DataElement01;

#define Swc_STOP_SEC_VAR_CLEARED_32
#include "Swc_MemMap.h"


#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"


// CHECK-MESSAGES: [[@LINE+1]]:16: warning: unconnected function interface 'Rte_Write01' [autosar-interface-connected]
Std_ReturnType Rte_Write01(uint32 Value)
{
    (void) Value;

    return RTE_E_UNCONNECTED;
}

// CHECK-MESSAGES: [[@LINE+1]]:16: warning: unconnected function interface 'Rte_Write02' [autosar-interface-connected]
Std_ReturnType Rte_Write02(uint32 Value)
{
    Std_ReturnType RetVal = RTE_E_UNCONNECTED;

    (void) Value;

    return RetVal;
}

Std_ReturnType Swc_Write_Data(uint32 Value)
{
    Std_ReturnType RetVal = RTE_E_UNCONNECTED;

    (void) Value;

    return RetVal;
}

// CHECK-MESSAGES: [[@LINE+1]]:16: warning: unconnected function interface 'Rte_Read01' [autosar-interface-connected]
Std_ReturnType Rte_Read01(uint32 *Value)
{
    *Value = 0;

    return RTE_E_UNCONNECTED;
}

// CHECK-MESSAGES: [[@LINE+1]]:16: warning: unconnected function interface 'Rte_Read02' [autosar-interface-connected]
Std_ReturnType Rte_Read02(uint32 *Value)
{
    Std_ReturnType RetVal = RTE_E_UNCONNECTED;

    *Value = Swc_DataElement01;

    return RetVal;
}

void Swc_Main1(void)
{
    Std_ReturnType Status;
    uint32 Value = 0u;

    Status = Rte_Write_PP_PortB_DataElement2(Value);

    (void) Rte_Write_PP_PortB_DataElement2(Value);

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: usage of unconnected macro interface 'Rte_Call_UnkownFault_SetEventStatus' [autosar-interface-connected]
    Status = Rte_Call_UnkownFault_SetEventStatus(DEM_EVENT_STATUS_PASSED);

    if (Status == RTE_E_UNCONNECTED) {}

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: usage of unconnected macro interface 'Rte_Call_UnkownFault_SetEventStatus' [autosar-interface-connected]
    (void) Rte_Call_UnkownFault_SetEventStatus(DEM_EVENT_STATUS_PASSED);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: usage of unconnected macro interface 'Rte_Call_UnkownFault_SetEventStatus' [autosar-interface-connected]
    Rte_Call_UnkownFault_SetEventStatus(DEM_EVENT_STATUS_PASSED);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: usage of unconnected macro interface 'Rte_Call_UnkownFault_SetEventStatus' [autosar-interface-connected]
    switch (Rte_Call_UnkownFault_SetEventStatus(EVENT_STATUS_FAILED)) {
    case RTE_E_OK:
        break;
    case RTE_E_INVALID:
        break;
    case RTE_E_UNCONNECTED:
        break;
    default:
        break;
    }
    do {
        // CHECK-MESSAGES: [[@LINE+1]]:16: warning: usage of unconnected macro interface 'Rte_Call_UnkownFault_SetEventStatus' [autosar-interface-connected]
        (void) Rte_Call_UnkownFault_SetEventStatus(DEM_EVENT_STATUS_PASSED);
    } while (0);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: usage of unconnected macro interface 'Rte_Read_RP_PortA_DataElement1' [autosar-interface-connected]
    (void) Rte_Read_RP_PortA_DataElement1(&Value);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: usage of unconnected macro interface 'Rte_Write_PP_PortB_DataElement1' [autosar-interface-connected]
    (void) Rte_Write_PP_PortB_DataElement1(Value);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: usage of unconnected macro interface 'Rte_Read_RP_PortA_DataElement1' [autosar-interface-connected]
    Rte_Read_RP_PortA_DataElement1(&Value);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: usage of unconnected macro interface 'Rte_Write_PP_PortB_DataElement1' [autosar-interface-connected]
    Rte_Write_PP_PortB_DataElement1(Value);

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: usage of unconnected macro interface 'Rte_Read_RP_PortA_DataElement1' [autosar-interface-connected]
    Status = Rte_Read_RP_PortA_DataElement1(&Value);

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: usage of unconnected macro interface 'Rte_Write_PP_PortB_DataElement1' [autosar-interface-connected]
    Status = Rte_Write_PP_PortB_DataElement1(Value);

    if (Rte_Write_PP_PortB_DataElement2(Value) == RTE_E_UNCONNECTED) {}

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: usage of unconnected macro interface 'Rte_Read_RP_PortA_DataElement1' [autosar-interface-connected]
    if (Rte_Read_RP_PortA_DataElement1(&Value) == RTE_E_UNCONNECTED) {}

}

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

