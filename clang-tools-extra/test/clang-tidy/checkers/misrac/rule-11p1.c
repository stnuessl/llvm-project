// RUN: %check_clang_tidy %s misrac-rule-11p1 %t

void g(void);

void f1(void) {
    void *p;
    void (*fp)(void);

    g();

    (void) &g;
    (void) fp;

    p = 0;
    p = (void *) 0;
    fp = (void *) 0;
    fp = &g;

    fp = 0;

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: cast involving 'void (*)(void)' and 'void *' [misrac-rule-11p1]
    fp = (void *) 1;

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: cast involving 'void (*)(void)' and 'int' [misrac-rule-11p1]
    fp = (void (*)(void)) 1;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: cast involving 'void *' and 'void (*)(void)' [misrac-rule-11p1]
    p = (void *) &g;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: cast involving 'void *' and 'void (*)(void)' [misrac-rule-11p1]
    p = (void *) fp;

    if (fp != (void *) 0) {}
    if (fp != (void (*)(void)) 0) {}
}

void f2(void *p) {
    void (*fp)(void);

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: cast involving 'void (*)(void)' and 'void *' [misrac-rule-11p1]
    fp = (void (*)(void)) p;
}
