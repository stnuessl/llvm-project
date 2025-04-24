// RUN: %check_clang_tidy %s misrac-rule-10p6 %t

typedef __UINT32_TYPE__ uint32;
typedef __UINT16_TYPE__ uint16;

typedef __INT32_TYPE__ sint32;
typedef __INT16_TYPE__ sint16;

void g(sint32 s32);

void f1(sint32 s32a, sint16 s16a) {
    sint32 s32b = s32a + 0;

    s32b = s32a;
    s32b = 0 + 0;
    s32b = (s32b + s32a);

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: assignment of 16-bit composite expression to wider 32-bit type [misrac-rule-10p6]
    s32b = s16a - 0;

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: assignment of 16-bit composite expression to wider 32-bit type [misrac-rule-10p6]
    s32b = (s16a - 0);
}

void f2(uint16 u16) {
    // CHECK-MESSAGES: [[@LINE+1]]:18: warning: assignment of 16-bit composite expression to wider 32-bit type [misrac-rule-10p6]
    uint32 u32 = u16 + u16;
}

uint32 f3(uint16 u16) {
    switch (u16) {
    case 0u:
        return u16;
    case 1u:
        // CHECK-MESSAGES: [[@LINE+1]]:16: warning: assignment of 16-bit composite expression to wider 32-bit type [misrac-rule-10p6]
        return 2u * u16;
    default:
        return 0u;
    }
}

void f4(sint16 s16) {
    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: assignment of 16-bit composite expression to wider 32-bit type [misrac-rule-10p6]
    g(1 * s16);

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: assignment of 16-bit composite expression to wider 32-bit type [misrac-rule-10p6]
    g(s16 + s16 + s16);

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: assignment of 16-bit composite expression to wider 32-bit type [misrac-rule-10p6]
    g((1 * s16) + (s16 + s16));

    g(1u << (s16 & 0x07));
}
