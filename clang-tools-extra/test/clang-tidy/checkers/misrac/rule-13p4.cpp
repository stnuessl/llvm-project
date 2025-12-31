// RUN: %check_clang_tidy %s misrac-rule-13p4 %t
// RUN: %check_clang_tidy -check-suffix=CXX %s misrac-rule-13p4 %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-13p4.ExtendToCPlusPlus: false \
// RUN:         } \
// RUN:     }'

int g();

void f() {
    int a, b;

    a = g();
    a = (b = g(), g());

    (a = g());
    ((a = g()));

    if (int x = g()) { (void) x; }

    // CHECK-MESSAGES: [[@LINE+1]]:11: warning: result of assignment used [misrac-rule-13p4]
    a = b = g();
    // CHECK-FIXES: {{^    }}a = b == g();

    // CHECK-MESSAGES: [[@LINE+1]]:22: warning: result of assignment used [misrac-rule-13p4]
    a = ((void) 0, b = g());
    // CHECK-FIXES: {{^    }}a = ((void) 0, b == g());

    // CHECK-MESSAGES: [[@LINE+1]]:36: warning: result of assignment used [misrac-rule-13p4]
    for (int i = 0; i < g(); i = a += g()) {}

    // CHECK-MESSAGES: [[@LINE+1]]:37: warning: result of assignment used [misrac-rule-13p4]
    for (int i = 0; i < g(); i = (a += g())) {}

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: result of assignment used [misrac-rule-13p4]
    if ((a = g())) {
    // CHECK-FIXES: {{^    }}if ((a == g())) {
        b = g();
        a += b * g();
    }

    // CHECK-MESSAGES: [[@LINE+2]]:15: warning: result of assignment used [misrac-rule-13p4]
    while (g())
        a = b <<= g();

    switch (g()) {
    case 0: a = g(); break;
    case 1: b = g(); break;
    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: result of assignment used [misrac-rule-13p4]
    default: a = b -= g(); break;
    }
}
