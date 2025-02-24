// RUN: %check_clang_tidy %s embedded-no-comma-operator %t
// RUN: %check_clang_tidy -check-suffix=STRICT %s embedded-no-comma-operator %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             embedded-no-comma-operator.StrictMode: true \
// RUN:         } \
// RUN:     }'
// RUN: %check_clang_tidy -check-suffix=REPORT %s embedded-no-comma-operator %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             embedded-no-comma-operator.ReportAtMacroExpansion: true \
// RUN:         } \
// RUN:     }'

// CHECK-MESSAGES: [[@LINE+2]]:28: warning: disallowed use of the comma operator [embedded-no-comma-operator]
// CHECK-MESSAGES-STRICT: [[@LINE+1]]:28: warning: disallowed use of the comma operator [embedded-no-comma-operator]
#define HIDDEN(x) ((void) x, (void)0)

void fn() {
    int a = 0, b = 0;
    int c, d;

    // CHECK-MESSAGES-STRICT: [[@LINE+1]]:49: warning: disallowed use of the comma operator [embedded-no-comma-operator]
    for (int i = 0, j = 0; i < 10 && j < 10; ++i, ++j) {
        // CHECK-MESSAGES: [[@LINE+3]]:18: warning: disallowed use of the comma operator [embedded-no-comma-operator]
        // CHECK-MESSAGES-STRICT: [[@LINE+2]]:18: warning: disallowed use of the comma operator [embedded-no-comma-operator]
        // CHECK-MESSAGES-REPORT: [[@LINE+1]]:18: warning: disallowed use of the comma operator [embedded-no-comma-operator]
        a += (++b, i + j);
    }

    // CHECK-MESSAGES: [[@LINE+3]]:10: warning: disallowed use of the comma operator [embedded-no-comma-operator]
    // CHECK-MESSAGES-STRICT: [[@LINE+2]]:10: warning: disallowed use of the comma operator [embedded-no-comma-operator]
    // CHECK-MESSAGES-REPORT: [[@LINE+1]]:10: warning: disallowed use of the comma operator [embedded-no-comma-operator]
    c = a, d = b;

    // CHECK-MESSAGES-REPORT: [[@LINE+1]]:5: warning: disallowed use of the comma operator [embedded-no-comma-operator]
    HIDDEN(c);
    // CHECK-MESSAGES-REPORT: [[@LINE+1]]:5: warning: disallowed use of the comma operator [embedded-no-comma-operator]
    HIDDEN(d);
}

