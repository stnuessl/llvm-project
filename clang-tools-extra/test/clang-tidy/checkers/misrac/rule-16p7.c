// RUN: %check_clang_tidy %s misrac-rule-16p7 %t

void f(int n) {
    _Bool b = (n != 0);

    switch (n) {
    case 0: break;
    case 1: break;
    default: break;
    }

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: switch condition has boolean value [misrac-rule-16p7]
    switch (n != 0) {
    case 0: break;
    case 1: break;
    default: break;
    }

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: switch condition has boolean value [misrac-rule-16p7]
    switch (n != 0 && n != 1) {
    case 0: break;
    case 1: break;
    default: break;
    }

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: switch condition has boolean value [misrac-rule-16p7]
    switch (b) {
    case 0: break;
    case 1: break;
    default: break;
    }
}
