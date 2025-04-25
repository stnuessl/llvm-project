// RUN: %check_clang_tidy %s misrac-rule-10p7 %t

typedef __UINT32_TYPE__ uint32;
typedef __UINT16_TYPE__ uint16;

typedef __INT32_TYPE__ sint32;
typedef __INT16_TYPE__ sint16;

void g(double f64);

void f1(uint32 u32, uint16 u16) {
    (void) (u32 + u32 + u16);
    (void) (u32 + u16 + u16);
    (void) (u16 + u32 + u16);
    (void) (u32 * u16 - u16);
    (void) ((u32 + u16) * u16);

    u16 += u32 - u16;

    (void) ((u16 * u16) - (u16 + u16));

    // CHECK-MESSAGES: [[@LINE+1]]:19: warning: narrower 16-bit composite expression used in operation with 32-bit operand [misrac-rule-10p7]
    (void) (u32 + u16 * u16);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: narrower 16-bit composite expression used in operation with 32-bit operand [misrac-rule-10p7]
    (void) (u16 + u16 + u32);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: narrower 16-bit composite expression used in operation with 32-bit operand [misrac-rule-10p7]
    u32 += u16 * u16;

    // CHECK-MESSAGES: [[@LINE+1]]:27: warning: narrower 16-bit composite expression used in operation with 32-bit operand [misrac-rule-10p7]
    (void) ((u32 * u16) - (u16 + u16));

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: narrower 16-bit composite expression used in operation with 32-bit operand [misrac-rule-10p7]
    (void) ((u16 * u16) - (u32 + u16));

    // CHECK-MESSAGES: [[@LINE+1]]:19: warning: narrower 16-bit composite expression used in operation with 32-bit operand [misrac-rule-10p7]
    (void) (u32 + ((0) ? u16: (u16 + u16)));
}

void f2(double f64, float f32) {
    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: narrower 32-bit composite expression used in operation with 64-bit operand [misrac-rule-10p7]
    g(2.0f * f64 + 4.0f * f32);
}
