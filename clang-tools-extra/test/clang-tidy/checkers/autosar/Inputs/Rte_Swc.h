#ifndef RTE_SWC_H
#define RTE_SWC_H

#include "Rte_Type.h"

typedef enum {
    SEVERITY_INFO,
    SEVERITY_WARNING,
    SEVERITY_ERROR,
    SEVERITY_FATAL,
} Severity;

typedef struct {
    uint32 Value;
} Swc_Internal;

#define RTE_START_SEC_CODE
#include "Generic_MemMap.h"

extern boolean Rte_Swc_OutPort_DataElement02;

#define Rte_Write_OutPort_DataElement02(Value) \
    (Rte_Swc_OutPort_DataElement02 = (Value), RTE_E_OK)

Std_ReturnType Rte_Read_InPort_DataElement01(uint8 *Data);
Std_ReturnType Rte_Write_OutPort_DataElement01(boolean Data);

Std_ReturnType NvM_GetErrorStatus(NvM_BlockIdType Id, NvM_RequestResultType *Result);
Std_ReturnType Dem_SetEventStatus(Dem_EventIdType Id, Dem_EventStatusType Status);

#define RTE_STOP_SEC_CODE
#include "Generic_MemMap.h"

#endif /* RTE_SWC_H */
