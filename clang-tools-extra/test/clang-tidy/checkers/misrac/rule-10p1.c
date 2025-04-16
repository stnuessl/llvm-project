// RUN: %check_clang_tidy %s misrac-rule-10p1 %t

_Static_assert(1, "");

enum e1 { E1_0, E1_1 };

void h1(unsigned int u);

void f1(const int *a) {
    (void) a[E1_0];
    (void) a[0];
    (void) a[0u];
    (void) sizeof(int);

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: essential type 'character' in array subscript expression [misrac-rule-10p1]
    (void) a['\0'];
}

void f2(void) {
    (void) +0.0;
    (void) +0;
    (void) +0u;
    (void) -0.0;
    (void) -0;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'unsigned' in unary minus [misrac-rule-10p1]
    (void) -0u;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'enum' in unary plus [misrac-rule-10p1]
    (void) +E1_0;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'boolean' in unary minus [misrac-rule-10p1]
    (void) -(_Bool) 1;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: essential type 'boolean' in unary minus [misrac-rule-10p1]
    h1(-(_Bool) 1);
}

void f3(void) {
    float f = 0.0;
    int i = 0;
    _Bool b = (_Bool) 0;

    (void) (0u + 0);
    (void) (0u + 0 + '\0' + 0.0);

    f++;
    --i;

    // CHECK-MESSAGES: [[@LINE+2]]:18: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:30: warning: essential type 'enum' in arithmetic operation [misrac-rule-10p1]
    (void) (0u + (_Bool) 0 - E1_0 + '\0');

    // CHECK-MESSAGES: [[@LINE+3]]:18: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+2]]:30: warning: essential type 'enum' in arithmetic operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:37: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    (void) (0u + (_Bool) 0 - E1_0 + (_Bool) 0);


    // CHECK-MESSAGES: [[@LINE+3]]:19: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+2]]:33: warning: essential type 'enum' in arithmetic operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:40: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    (void) ((0u + (_Bool) 0) - (E1_0 + (_Bool) 0));

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: essential type 'character' in arithmetic operation [misrac-rule-10p1]
    0u * '\0';

    // CHECK-MESSAGES: [[@LINE+1]]:25: warning: essential type 'enum' in arithmetic operation [misrac-rule-10p1]
    (void) (0u + 0.0f / E1_1);

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    --b;

    // CHECK-MESSAGES: [[@LINE+2]]:13: warning: essential type 'character' in modulo operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: essential type 'boolean' in modulo operation [misrac-rule-10p1]
    (void) ('\0' % (_Bool) 1);

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    i += (_Bool) 0;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    b *= 0;

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    i = i + (0 == 0);

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: essential type 'boolean' in arithmetic operation [misrac-rule-10p1]
    h1(1 + (_Bool) 1 / 1.0f);
}

void f4(void) {
    (void) (0 < 0.0);
    (void) (0 <= '\0');
    (void) (0u > E1_0);

    if (0u >= 0.0f)
        return;

    // CHECK-MESSAGES: [[@LINE+1]]:19: warning: essential type 'boolean' in relational operation [misrac-rule-10p1]
    (void) (0u >= (_Bool) 0);


    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'boolean' in relational operation [misrac-rule-10p1]
    (void) ((_Bool) 0 < 0u);
}

void f5(void) {
    (void) !(_Bool) 0;
    (void) ((_Bool) 0 && (_Bool) 1);
    (void) ((_Bool) 1 || (_Bool) 0);

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: essential type 'unsigned' in logical operation [misrac-rule-10p1]
    if (0u && (_Bool) 0) {}

    // CHECK-MESSAGES: [[@LINE+2]]:13: warning: essential type 'unsigned' in logical operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:19: warning: essential type 'floating' in logical operation [misrac-rule-10p1]
    (void) (0u && 0.0f);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'character' in logical operation [misrac-rule-10p1]
    (void) !'\0';

    // CHECK-MESSAGES: [[@LINE+1]]:26: warning: essential type 'enum' in logical operation [misrac-rule-10p1]
    (void) ((_Bool) 1 && E1_0);
}

unsigned int f6(void) {
    unsigned int u = 0u;
    _Bool b = 0;

    u <<= 1u;
    u >>= 1u;
    u = 0u << 1u;
    u = 0u >> 1u;

    (void) (0u << 0u);
    (void) (0u >> 0u);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'signed' in shift operation [misrac-rule-10p1]
    (void) (0 << 0u);

    // CHECK-MESSAGES: [[@LINE+2]]:13: warning: essential type 'signed' in shift operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:18: warning: essential type 'enum' in shift operation [misrac-rule-10p1]
    (void) (0 >> E1_0);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: essential type 'boolean' in shift operation [misrac-rule-10p1]
    b <<= 1u;

    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: essential type 'boolean' in shift operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:11: warning: essential type 'signed' in shift operation [misrac-rule-10p1]
    b <<= 1;

    // CHECK-MESSAGES: [[@LINE+2]]:12: warning: essential type 'boolean' in shift operation [misrac-rule-10p1]
    // CHECK-MESSAGES: [[@LINE+1]]:17: warning: essential type 'signed' in shift operation [misrac-rule-10p1]
    return b << 1;
}

void f7(void) {
    // CHECK-MESSAGES: [[@LINE+1]]:22: warning: essential type 'signed' in conditional operation [misrac-rule-10p1]
    unsigned int u = 0 ? 0 : 1;

    (void) u;

    (void) ((_Bool) 0 ? 0 : 1);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'signed' in conditional operation [misrac-rule-10p1]
    (void) ((0) ? 0 : 1);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'floating' in conditional operation [misrac-rule-10p1]
    (void) (0.0f ? 0 : 1);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type 'enum' in conditional operation [misrac-rule-10p1]
    (void) (E1_0 ? 0 : 1);
}
