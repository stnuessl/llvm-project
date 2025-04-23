// RUN: %check_clang_tidy %s misrac-rule-10p4 %t
// RUN: %check_clang_tidy -check-suffix=CONFIG %s misrac-rule-10p4 %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-10p4.IgnoreAssignments: true, \
// RUN:         } \
// RUN:     }'

void f1(void) {
    (void) (0 + 0);
    (void) (0u - 0u);
    (void) (0.0 * 0.0f);

    // CHECK-MESSAGES: [[@LINE+2]]:18: warning: use of different type categories 'floating' and 'unsigned' in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:18: warning: use of different type categories 'floating' and 'unsigned' in operation [misrac-rule-10p4]
    (void) (0.0f * 0u);

    // CHECK-MESSAGES: [[@LINE+2]]:32: warning: use of different type categories 'boolean' and 'signed' in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:32: warning: use of different type categories 'boolean' and 'signed' in operation [misrac-rule-10p4]
    (void) (1 + (0 ? (_Bool) 0 : 1));

    // CHECK-MESSAGES: [[@LINE+2]]:26: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:26: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    (void) (0u + 0u - 0u + 0);

    // CHECK-MESSAGES: [[@LINE+2]]:15: warning: use of different type categories 'signed' and 'unsigned' in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:15: warning: use of different type categories 'signed' and 'unsigned' in operation [misrac-rule-10p4]
    (void) (0 % 2u);

    // CHECK-MESSAGES: [[@LINE+2]]:15: warning: use of different type categories 'signed' and 'unsigned' in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:15: warning: use of different type categories 'signed' and 'unsigned' in operation [misrac-rule-10p4]
    (void) (0 == 0u);

    // CHECK-MESSAGES: [[@LINE+2]]:15: warning: use of different type categories 'signed' and 'unsigned' in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:15: warning: use of different type categories 'signed' and 'unsigned' in operation [misrac-rule-10p4]
    (void) (0 >= 0u);

    // CHECK-MESSAGES: [[@LINE+2]]:21: warning: use of different type categories 'signed' and 'floating' in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:21: warning: use of different type categories 'signed' and 'floating' in operation [misrac-rule-10p4]
    (void) sizeof(0 + 0.0f);
}

void f2(void) {
    unsigned int u = 0u;

    u <<= 1;
    u >>= 1;

    if (u && (int) !u) {}
    if (u || !(int) u) {}

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u += 0;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u -= 0;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u *= 0;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u /= 1;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u %= 1;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u &= 1;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u |= 1;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u ^= 1;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: use of different type categories 'unsigned' and 'signed' in operation [misrac-rule-10p4]
    u += (0 * 0 * 0);
}

void f3(void) {
    enum e1 { E1_0 };
    enum e2 { E2_0 };

    char c;

    c = '\0' + 0u;
    c = 0 + '\0';
    c = '\0' - 0;

    c += 0;
    c -= 0;

    // CHECK-MESSAGES: [[@LINE+2]]:18: warning: use of different enumeration types in operation [misrac-rule-10p4]
    // CHECK-MESSAGES-CONFIG: [[@LINE+1]]:18: warning: use of different enumeration types in operation [misrac-rule-10p4]
    (void) (E1_0 < E2_0);
}
