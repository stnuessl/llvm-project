// RUN: %check_clang_tidy %s misc-no-variable-array %t

const int n1 = 100;
int a1[n1 + 50];

void fn1() {
    int a2[n1];
}

void fn2(int n) {
    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: disallowed use of variable length array 'a3' [misc-no-variable-array]
    int a3[n];
    // CHECK-FIXES: {{^    }};
}
