// RUN: %check_clang_tidy %s misrac-rule-16p3 %t

void f(int n) {

    switch (n) {
    case 0: break;
    case 1: break;
    default: break;
    }

    switch (n) {
    case 0: { break; }
    case 1: break;
    default: break;
    }

    switch (n) {
    case 0: (void) 0; break;
    case 1: break;
    default: break;
    }

    switch (n) {
    case 0:
    case 1:
        (void) 0;
        (void) 1;
        break;
    default: break;
    }

    switch (n) {
    case 0: {} break;
    case 1: break;
    default: break;
    }

    switch (n) {
    case 0:
    case 1: break;
    default: break;
    }

    switch (n) {
    case 0:
    case 1:
    default: break;
    }

    switch (n) break;
    switch (n) case 0: break;
    switch (n) case 0: { break; };

    switch (n) {
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: missing unconditional break statement in case [misrac-rule-16p3]
    case 0:
        (void) 0;
        (void) 1;
        /* FALLTHROUGH */
    case 1: break;
    default: break;
    }

    switch (n) {
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: missing unconditional break statement in case [misrac-rule-16p3]
    case 0: {
        (void) 0;
        (void) 1;
        /* FALLTHROUGH */
    }
    case 1: break;
    default: break;
    }

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: missing unconditional break statement in case [misrac-rule-16p3]
    switch (n) case 0: (void) 0;

    switch (n) {
    case 0:
    case 1:
        (void) 0;
        (void) 1;
        break;
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: missing unconditional break statement in case [misrac-rule-16p3]
    default: (void) 0;
    }

}
