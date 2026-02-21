// RUN: %check_clang_tidy %s misrac-rule-11p1 %t

void g();

void f() {
    void *p;
    void (*fp)();

    p = 0;
    p = (void *) 0;
    fp = 0;
    fp = &g;
    fp = (void (*)()) 0;

    (void) p;
    (void) fp;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: cast involving 'void *' and 'void (*)()' [misrac-rule-11p1]
    p = (void *) &g;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: cast involving 'void *' and 'void (*)()' [misrac-rule-11p1]
    p = (void *) fp;

}
