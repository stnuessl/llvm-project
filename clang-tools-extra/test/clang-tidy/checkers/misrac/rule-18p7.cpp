// RUN: %check_clang_tidy %s misrac-rule-18p7 %t

struct x {
    // CHECK-MESSAGES: [[@LINE+2]]:10: warning: flexible array member used [misrac-rule-18p7]
    int size;
    char data[];
};

extern int a[];

void f(int n) {
    int b[n];
}

