// RUN: %check_clang_tidy %s embedded-no-flexible-array-member %t

struct x {
    // CHECK-MESSAGES: [[@LINE+2]]:10: warning: forbidden use of flexible array member [embedded-no-flexible-array-member]
    int size;
    char data[];
};

extern int a[];

void f(int n) {
    int b[n];
}
