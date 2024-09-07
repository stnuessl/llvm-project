// RUN: %check_clang_tidy %s autosar-type-usage %t -- -- -I%S/Inputs

#include "Rte_Swc.h"

class c {
public:
    bool operator==(const c &other) const { return false; }
};

#define Swc_START_SEC_CODE
#include "Swc_MemMap.h"

static bool operator!=(const c &lhs, const c &rhs) {
    return false;
}

template <typename T>
void Swc_Template(T x)
{
    (void)x;
}

template <typename T, typename U>
void Swc_Template(const T &x, const U &y)
{
    (void)x;
    (void)y;
}

void Rte_Call_Variadic(int n, ...);

void Swc_Main(void)
{
    Std_ReturnType RetVal = E_NOT_OK;

    Swc_Template(E_OK);
    Swc_Template(RetVal, E_OK);

    Rte_Call_Variadic(2, 0, 0);
    Rte_Call_Variadic(0);

    (void) (c() == c());
    (void) (c() != c());
}


#define Swc_STOP_SEC_CODE
#include "Swc_MemMap.h"

