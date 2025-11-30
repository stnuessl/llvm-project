// RUN: %check_clang_tidy %s misrac-rule-16p4 %t

void f(int n) {
    switch (n) {
    case 0: break;
    case 1: break;
    default: break;
    }

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: missing 'default' label [misrac-rule-16p4]
    switch (n) {
    case 0: break;
    case 1: break;
    // CHECK-FIXES: {{^    }}default:
    }
}

