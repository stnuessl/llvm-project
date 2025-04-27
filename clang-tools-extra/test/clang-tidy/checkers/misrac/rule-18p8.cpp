// RUN: %check_clang_tidy %s misrac-rule-18p8 %t

struct x {
    int size;
    char data[];
};

int a1[10];

void f(int n) {
    int a2[10];

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: variadic length array used [misrac-rule-18p8]
    int a3[n];
}

