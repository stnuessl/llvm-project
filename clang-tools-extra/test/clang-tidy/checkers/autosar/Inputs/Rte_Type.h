#ifndef RTE_TYPE_H
#define RTE_TYPE_H

#include "Std_Types.h"

typedef uint16 NvM_BlockIdType;
typedef uint8 NvM_RequestResultType;

typedef uint16 Dem_EventIdType;
typedef uint8 Dem_EventStatusType;

#define NVM_REQ_OK 0u
#define NVM_REQ_NOT_OK 1u
#define NVM_REQ_PENDING 2u

#define DEM_EVENT_STATUS_PASSED 0u
#define DEM_EVENT_STATUS_FAILED 1u
#define DEM_EVENT_STATUS_PREPASSED 2u
#define DEM_EVENT_STATUS_PREFAILED 3u

#endif /* RTE_TYPE_H */
