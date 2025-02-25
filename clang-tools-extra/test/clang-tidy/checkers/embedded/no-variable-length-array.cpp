// RUN: %check_clang_tidy %s embedded-no-variable-length-array %t

struct x {
    int size;
    char data[];
};

int a1[10];

void f1(int n) {
    int a2[10];

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: forbidden use of variadic length array [embedded-no-variable-length-array]
    int a3[n];
}
