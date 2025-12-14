// RUN: %check_clang_tidy %s misrac-rule-16p2 %t

void f(int n, int m) {
    switch (n) {
    case 0: break;
    case 1: break;
    default: break;
    }

    switch (n) {
    case 0: break;
    case 1: /* FALLTHROUGH */
    default: break;
    }

    switch (n) {
    case 0:
        if (m) {
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: invalid enclosing parent of switch case [misrac-rule-16p2]
    case 1:
            ;
        }
        break;
    default: break;
    }

    switch (n) {
    case 0:
        if (m) {
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: invalid enclosing parent of switch case [misrac-rule-16p2]
    default:
            ;
        }
        break;
    }
}
