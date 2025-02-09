// RUN: %check_clang_tidy %s autosar-macro-interface %t -- -- -I%S/Inputs

#include "Rte_Swc.h"

#define Rte_Read_InPort_DataElement03 \
    Rte_Read_Swc_InPort_DataElement03

#define Rte_Call_DTC_SetEventStatus(Status) \
    Dem_SetEventStatus((Dem_EventIdType) 0u, Status)

#define Swc_Write_OutPort_DataElement02(Value) \
    Rte_Write_OutPort_DataElement02(Value)


Std_ReturnType Rte_Read_Swc_InPort_DataElement03(uint32 *Value);

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Main(void)
{
    uint32 Value = 0u;

    (void) Swc_Write_OutPort_DataElement02(FALSE);
    (void) Rte_Write_OutPort_DataElement02(FALSE);

    (void) Rte_Read_Swc_InPort_DataElement03(&Value);
    (void) Rte_Read_InPort_DataElement03(&Value);
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"
