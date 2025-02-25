// RUN: %check_clang_tidy %s embedded-missing-default-label %t

// FIXME: Add something that triggers the check here.
void f(int n) {
    switch (n) {
    case 0: break;
    case 1: break;
    default: break;
    }

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: 'switch' missing 'default' label [embedded-missing-default-label]
    switch (n) {
    case 0: break;
    case 1: break;
    // CHECK-FIXES: {{^    }}default: break;
    }
}

