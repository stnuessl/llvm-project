// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Type.h"

struct ArData {
    boolean Init;
    uint32 Value;
    NvM_RequestResultType NvmResult;
};

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

void Swc_Variables01(void)
{
    boolean Value01 = FALSE;

    // CHECK-MESSAGES: [[@LINE+1]]:23: warning: a value of type 'unsigned int' cannot be used to initialize a variable of type 'boolean' [autosar-type-usage]
    boolean Value02 = 0u;

    // CHECK-MESSAGES: [[@LINE+1]]:23: warning: a value of type 'Std_ReturnType' cannot be used to initialize a variable of type 'boolean' [autosar-type-usage]
    boolean Value03 = E_OK;

    // CHECK-MESSAGES: [[@LINE+1]]:21: warning: a value of type 'boolean' cannot be used to initialize a variable of type 'uint8' [autosar-type-usage]
    uint8 Value04 = FALSE;

    // CHECK-MESSAGES: [[@LINE+1]]:29: warning: invalid explicit cast involving type 'boolean' [autosar-type-usage]
    uint8 Value05 = (uint8) FALSE;
}


void Swc_Record01(void)
{
    struct ArData Data01 = { FALSE, 0u, NVM_REQ_OK };

    // CHECK-MESSAGES: [[@LINE+2]]:30: warning: a value of type 'unsigned int' cannot be used to initialize a member variable of type 'boolean' [autosar-type-usage]
    // CHECK-MESSAGES: [[@LINE+1]]:38: warning: a value of type 'unsigned int' cannot be used to initialize a member variable of type 'NvM_RequestResultType' [autosar-type-usage]
    struct ArData Data02 = { 0u, 0u, 0u };
}

void Swc_Array01(void)
{
    boolean Data01[] = { FALSE, FALSE, FALSE };

    // CHECK-MESSAGES: [[@LINE+2]]:33: warning: a value of type 'unsigned int' cannot be used to initialize an array member of type 'boolean' [autosar-type-usage]
    // CHECK-MESSAGES: [[@LINE+1]]:37: warning: a value of type 'Std_ReturnType' cannot be used to initialize an array member of type 'boolean' [autosar-type-usage]
    boolean Data02[] = { FALSE, 0u, E_OK };
}

void Swc_NestedData01(void)
{
    struct ArData Data01[] = {
        { FALSE, 0u, NVM_REQ_OK },
        // CHECK-MESSAGES: [[@LINE+2]]:11: warning: a value of type 'unsigned int' cannot be used to initialize a member variable of type 'boolean' [autosar-type-usage]
        // CHECK-MESSAGES: [[@LINE+1]]:19: warning: a value of type 'unsigned int' cannot be used to initialize a member variable of type 'NvM_RequestResultType' [autosar-type-usage]
        { 0u, 0u, 0u},
        // CHECK-MESSAGES: [[@LINE+2]]:19: warning: a value of type 'unsigned int' cannot be used to initialize a member variable of type 'boolean' [autosar-type-usage]
        // CHECK-MESSAGES: [[@LINE+1]]:32: warning: a value of type 'boolean' cannot be used to initialize a member variable of type 'uint32' [autosar-type-usage]
        { .Init = 0u, .Value = FALSE, NVM_REQ_NOT_OK },
    };

    Dem_EventStatusType Data02[][2] = {
        { DEM_EVENT_STATUS_FAILED, DEM_EVENT_STATUS_PASSED },
        // CHECK-MESSAGES: [[@LINE+1]]:17: warning: a value of type 'NvM_RequestResultType' cannot be used to initialize an array member of type 'Dem_EventStatusType' [autosar-type-usage]
        { [0] = NVM_REQ_OK, [1] = DEM_EVENT_STATUS_PREFAILED },
        // CHECK-MESSAGES: [[@LINE+1]]:39: warning: a value of type 'unsigned int' cannot be used to initialize an array member of type 'Dem_EventStatusType' [autosar-type-usage]
        { DEM_EVENT_STATUS_PREPASSED, 0u },
    };

    struct {
        struct ArData Data[3][3];
    } Table[] = {
        [0] = {
            {
                [0] = {
                    { FALSE, 0u, NVM_REQ_OK },
                    { FALSE, 0u, NVM_REQ_OK },
                    { FALSE, 0u, NVM_REQ_OK },
                },
                [1] = {
                    // CHECK-MESSAGES: [[@LINE+1]]:34: warning: a value of type 'Std_ReturnType' cannot be used to initialize a member variable of type 'NvM_RequestResultType' [autosar-type-usage]
                    { FALSE, 0u, E_OK },
                    // CHECK-MESSAGES: [[@LINE+1]]:30: warning: a value of type 'Std_ReturnType' cannot be used to initialize a member variable of type 'uint32' [autosar-type-usage]
                    { FALSE, E_OK, NVM_REQ_OK },
                    // CHECK-MESSAGES: [[@LINE+1]]:23: warning: a value of type 'Std_ReturnType' cannot be used to initialize a member variable of type 'boolean' [autosar-type-usage]
                    { E_OK, 0u, NVM_REQ_OK },
                },
                [2] = {
                    { FALSE, 0u, NVM_REQ_OK },
                    { FALSE, 0u, NVM_REQ_OK },
                    { FALSE, 0u, NVM_REQ_OK },
                },
            },
        },
    };
}

#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"
