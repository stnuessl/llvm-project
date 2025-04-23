// RUN: %check_clang_tidy %s misrac-rule-10p3 %t

#define FALSE ((_Bool) 0)
#define TRUE ((_Bool) 1)

typedef __UINT32_TYPE__ uint32;
typedef __UINT16_TYPE__ uint16;
typedef __UINT8_TYPE__ uint8;

typedef __INT32_TYPE__ sint32;
typedef __INT16_TYPE__ sint16;
typedef __INT8_TYPE__ sint8;

typedef float float32;
typedef double float64;

typedef _Bool boolean;

enum { E0_0 = 0 };
enum { E0_1 = 0x7FFFFFFF };
enum e1 { E1_0 = 0x7FFFFFFF };

void g1(sint16 s16);
void g2(sint32 *p);

void f1(void) {
    boolean b;
    sint32 s32;
    uint32 u32;
    sint16 s16;
    uint16 u16;

    s32 = 0;
    u32 = 0u;
    s16 = 0;
    b = FALSE;
    b = (boolean) 0;

    s32 = 0 + 0;
    u32 = 0u * 0u;
    s16 = 0 - 0;
    b = (0 != 0);

    s16 = E0_0;

    s16 = 32767;
    s16 = 32767 + 1 - 1;
    s16 = -32768;

    u16 = 65535u;
    u16 = 65535u + 1u - 1u;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: use of different type categories 'signed' and 'unsigned' in assignement [misrac-rule-10p3]
    s32 = 0u;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: use of different type categories 'unsigned' and 'boolean' in assignement [misrac-rule-10p3]
    u32 = (boolean) 0;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: use of different type categories 'signed' and 'boolean' in assignement [misrac-rule-10p3]
    s32 = (0 == 0);

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: assignment of 32-bit value to narrower 16-bit value [misrac-rule-10p3]
    s16 = s32;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: assignment of 32-bit value to narrower 16-bit value [misrac-rule-10p3]
    s16 = E0_1;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: assignment of 32-bit value to narrower 16-bit value [misrac-rule-10p3]
    s16 = 32768;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: assignment of 32-bit value to narrower 16-bit value [misrac-rule-10p3]
    s16 = 32767 + 1;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: assignment of 32-bit value to narrower 16-bit value [misrac-rule-10p3]
    u16 = 65535u + 1u;
}

void f2(void) {
    sint32 s32 = 0;

    g1(0);
    g1((sint16) sizeof(0));
    g2((sint32 *) 0);

    // CHECK-MESSAGES: [[@LINE+1]]:8: warning: use of different type categories 'signed' and 'floating' in assignement [misrac-rule-10p3]
    g1(0.0f);

    // CHECK-MESSAGES: [[@LINE+1]]:8: warning: use of different type categories 'signed' and 'floating' in assignement [misrac-rule-10p3]
    g1(0.0f + 0);

    // CHECK-MESSAGES: [[@LINE+1]]:8: warning: assignment of 32-bit value to narrower 16-bit value [misrac-rule-10p3]
    g1(s32);

    // CHECK-MESSAGES: [[@LINE+1]]:8: warning: use of different type categories 'signed' and 'boolean' in assignement [misrac-rule-10p3]
    g1(0 != 0);

    // CHECK-MESSAGES: [[@LINE+1]]:8: warning: use of different type categories 'signed' and 'unsigned' in assignement [misrac-rule-10p3]
    g1(sizeof(0));
}

uint8 f3(uint32 u32) {
    switch (u32) {
    case 0u:
        return 0u;
    case 1u:
        // CHECK-MESSAGES: [[@LINE+1]]:16: warning: assignment of 32-bit value to narrower 8-bit value [misrac-rule-10p3]
        return u32;
    case 2u:
        // CHECK-MESSAGES: [[@LINE+1]]:16: warning: use of different type categories 'unsigned' and 'signed' in assignement [misrac-rule-10p3]
        return 0 + 0;
    case 3u:
        return (uint8) E0_0;
    case 4u:
        // CHECK-MESSAGES: [[@LINE+1]]:16: warning: use of different type categories 'unsigned' and 'boolean' in assignement [misrac-rule-10p3]
        return 0 == 0 && 1 == 1;
    case 5u:
        // CHECK-MESSAGES: [[@LINE+1]]:16: warning: use of different type categories 'unsigned' and 'enum' in assignement [misrac-rule-10p3]
        return E1_0;
    default:
        return f3(0u);
    }
}

void f4(void) {
    const struct {
        sint16 data[3][3];
        uint32 u32;
    } s[] = {
        {
            .data = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}},
            .u32 = 0u,
        },
        // CHECK-MESSAGES: [[@LINE+3]]:37: warning: use of different type categories 'signed' and 'unsigned' in assignement [misrac-rule-10p3]
        // CHECK-MESSAGES: [[@LINE+3]]:20: warning: use of different type categories 'unsigned' and 'signed' in assignement [misrac-rule-10p3]
        {
            .data = {{0, 1, 2}, {0, 1u, 2}, {0, 1, 2}},
            .u32 = 0 ? 0 + 0 : 0,
        },
        // CHECK-MESSAGES: [[@LINE+2]]:40: warning: assignment of 32-bit value to narrower 16-bit value [misrac-rule-10p3]
        {
            .data = {{0, 1, 2}, {0, 1, 32768}, {0, 1, 2}},
            .u32 = 0u,
        },
    };
}

