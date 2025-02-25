// RUN: %check_clang_tidy %s embedded-no-stdarg-features %t

#include <stdarg.h>

void f1(int n, ...) {
    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: forbidden use of type 'va_list' [embedded-no-stdarg-features]
    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: forbidden use of type 'va_list' [embedded-no-stdarg-features]
    va_list vargs;
    va_list vargs_copy;
    int value;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: forbidden use of macro 'va_start' [embedded-no-stdarg-features]
    va_start(vargs, n);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: forbidden use of macro 'va_arg' [embedded-no-stdarg-features]
    value = va_arg(vargs, int);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: forbidden use of macro 'va_copy' [embedded-no-stdarg-features
    va_copy(vargs_copy, vargs);

    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: forbidden use of macro 'va_end' [embedded-no-stdarg-features]
    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: forbidden use of macro 'va_end' [embedded-no-stdarg-features]
    va_end(vargs_copy);
    va_end(vargs);
}

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: forbidden use of type 'va_list' [embedded-no-stdarg-features
void f2(va_list vargs);
