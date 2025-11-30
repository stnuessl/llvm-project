// RUN: %check_clang_tidy %s misrac-rule-16p5 %t

void g();

void f(int n) {

    switch (n) {
    default: break;
    case 0: break;
    case 1: break;
    }

    switch (n) {
    default: /* FALLTHROUGH */
    case 0: /* FALLTHROUGH */
    case 1: break;
    }

    switch (n) {
    g();
    default: {}
    case 0: break;
    case 1: break;
    }

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

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: the default case must either be the first or last case in a switch statement [misrac-rule-16p5]
    switch (n) {
    case 0: break;
    default: break;
    case 1: break;
    }
}
