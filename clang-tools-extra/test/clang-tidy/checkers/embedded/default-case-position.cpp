// RUN: %check_clang_tidy %s embedded-default-case-position %t

void f(int n) {

    switch (n) {
    default: break;
    case 0: break;
    case 1: break;
    }

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: default case must be at first or last position in switch [embedded-default-case-position]
    switch (n) {
    case 0: break;
    default: break;
    case 1: break;
    }

    switch (n) {
    case 0: break;
    case 1: break;
    default: break;
    }
}
