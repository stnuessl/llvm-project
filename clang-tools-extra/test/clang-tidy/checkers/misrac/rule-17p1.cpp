// RUN: %check_clang_tidy %s misrac-rule-17p1 %t

#include <stdarg.h>

void f1(int n, ...) {
    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: use of type 'va_list' [misrac-rule-17p1]
    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: use of type 'va_list' [misrac-rule-17p1]
    va_list vargs;
    va_list vargs_copy;
    int value;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: use of macro 'va_start' [misrac-rule-17p1]
    va_start(vargs, n);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: use of macro 'va_arg' [misrac-rule-17p1]
    value = va_arg(vargs, int);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: use of macro 'va_copy' [misrac-rule-17p1
    va_copy(vargs_copy, vargs);

    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: use of macro 'va_end' [misrac-rule-17p1]
    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: use of macro 'va_end' [misrac-rule-17p1]
    va_end(vargs_copy);
    va_end(vargs);
}

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: use of type 'va_list' [misrac-rule-17p1
void f2(va_list vargs);
