// RUN: %check_clang_tidy %s misrac-rule-11p5 %t

#define NULL ((void *) 0)

void f(void) {
    void *pv = 0;
    int *pi = 0;

    pv = (void *) 0;
    pi = (void *) 0;
    pv = NULL;
    pi = NULL;

    pv = pi;
    pv = (void *) pi;

    if (pi != NULL) {}

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: implicit cast from 'void *' to 'int *' [misrac-rule-11p5]
    pi = pv;

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: cast from 'void *' to 'int *' [misrac-rule-11p5]
    pi = (int *) pv;

}

