#ifndef SWC_H
#define SWC_H

#include "Std_Types.h"

#define Swc_START_SEC_VAR_INIT_UNSPECIFIED
#include "Swc_MemMap.h"

extern uint32 Swc_GlobalData;

#define Swc_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Swc_MemMap.h"

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

Std_ReturnType Swc_GetStatus(void);

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

#endif // SWC_H
