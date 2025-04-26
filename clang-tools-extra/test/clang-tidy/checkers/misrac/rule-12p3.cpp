// RUN: %check_clang_tidy %s misrac-rule-12p3 %t
// RUN: %check_clang_tidy -check-suffix=FORLOOP %s misrac-rule-12p3 %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-12p3.ReportUseInForLoops: true \
// RUN:         } \
// RUN:     }'
// RUN: %check_clang_tidy -check-suffix=MACRO %s misrac-rule-12p3 %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-12p3.ReportAtMacroDefinition: true \
// RUN:         } \
// RUN:     }'

// CHECK-MESSAGES-MACRO: [[@LINE+1]]:28: warning: comma operator used [misrac-rule-12p3]
#define HIDDEN(x) ((void) x, (void)0)

void fn() {
    int a = 0, b = 0;
    int c, d;

    // CHECK-MESSAGES-FORLOOP: [[@LINE+1]]:49: warning: comma operator used [misrac-rule-12p3]
    for (int i = 0, j = 0; i < 10 && j < 10; ++i, ++j) {
        // CHECK-MESSAGES: [[@LINE+3]]:18: warning: comma operator used [misrac-rule-12p3]
        // CHECK-MESSAGES-FORLOOP: [[@LINE+2]]:18: warning: comma operator used [misrac-rule-12p3]
        // CHECK-MESSAGES-MACRO: [[@LINE+1]]:18: warning: comma operator used [misrac-rule-12p3]
        a += (++b, i + j);
    }

    // CHECK-MESSAGES: [[@LINE+3]]:10: warning: comma operator used [misrac-rule-12p3]
    // CHECK-MESSAGES-FORLOOP: [[@LINE+2]]:10: warning: comma operator used [misrac-rule-12p3]
    // CHECK-MESSAGES-MACRO: [[@LINE+1]]:10: warning: comma operator used [misrac-rule-12p3]
    c = a, d = b;

    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: comma operator used [misrac-rule-12p3]
    // CHECK-MESSAGES-FORLOOP: [[@LINE+1]]:5: warning: comma operator used [misrac-rule-12p3]
    HIDDEN(c);

    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: comma operator used [misrac-rule-12p3]
    // CHECK-MESSAGES-FORLOOP: [[@LINE+1]]:5: warning: comma operator used [misrac-rule-12p3]
    HIDDEN(d);
}

