#ifndef RTE_TYPE_H
#define RTE_TYPE_H

#include "Std_Types.h"

#define RTE_E_UNCONNECTED 134u

#define NVM_REQ_OK 0u
#define NVM_REQ_NOT_OK 1u
#define NVM_REQ_PENDING 2u

#define DEM_EVENT_STATUS_PASSED 0u
#define DEM_EVENT_STATUS_FAILED 1u
#define DEM_EVENT_STATUS_PREPASSED 2u
#define DEM_EVENT_STATUS_PREFAILED 3u

#define COMM_NO_COMMUNICATION 0u
#define COMM_SILENT_COMMUNICATION 1u
#define COMM_FULL_COMMUNICATION 2u

#define DCM_E_PENDING (Std_ReturnType) 10u
#define DCM_E_FORCE_RCRRP (Std_ReturnType) 12u

typedef uint16 NvM_BlockIdType;
typedef uint8 NvM_RequestResultType;

typedef uint16 Dem_EventIdType;
typedef uint8 Dem_EventStatusType;

typedef uint8 ComM_ModeType;

#endif /* RTE_TYPE_H */
