// RUN: %check_clang_tidy %s misrac-rule-11p8 %t

typedef void *void_ptr;
typedef const void *void_ptr_const;

void f1(const int *p1, volatile unsigned char *p2) {
    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: cast removes 'const' qualifier [misrac-rule-11p8]
    int *q1 = (int *) p1;

    // CHECK-MESSAGES: [[@LINE+1]]:25: warning: cast removes 'volatile' qualifier [misrac-rule-11p8]
    unsigned char *q2 = (unsigned char *) p2;
}

void f3(const volatile int *p) {
    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: cast removes 'const' and 'volatile' qualifiers [misrac-rule-11p8]
    int *q = (int *) p;
}

void f3(void_ptr_const p) {
    // CHECK-MESSAGES: [[@LINE+1]]:19: warning: cast removes 'const' qualifier [misrac-rule-11p8]
    void_ptr *q = (void_ptr *) p;
}

void f4(int *const *p) {
    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: cast removes 'const' qualifier [misrac-rule-11p8]
    int **q = (int **) p;
}
