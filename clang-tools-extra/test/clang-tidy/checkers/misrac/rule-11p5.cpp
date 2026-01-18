// RUN: %check_clang_tidy %s misrac-rule-11p5 %t

void f() {
    void *pv = nullptr;
    int *pi = nullptr;

    pv = (void *) 0;
    pi = (int *) 0;
    pv = pi;

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: cast from 'void *' to 'int *' [misrac-rule-11p5]
    pi = (int *) pv;
}
