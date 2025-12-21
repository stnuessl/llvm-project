// RUN: %check_clang_tidy %s misrac-rule-17p7 %t

int f(void);

void g(int n) {
    (void) f();

    if (f()) { }
    while (f()) {}
    do { } while (f());

    switch (f()) {
    case 0: break;
    default: break;
    }

    (void) ((void) 0, f());

    (void) (f() ? 1 : 0);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: unused return value [misrac-rule-17p7]
    f();
    // CHECK-FIXES: {{^    }}(void) f();

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: unused return value [misrac-rule-17p7]
    if (n) { f(); }
    // CHECK-FIXES: {{^    }}if (n) { (void) f(); }

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: unused return value [misrac-rule-17p7]
    if (n) f();
    // CHECK-FIXES: {{^    }}if (n) (void) f();

    // CHECK-MESSAGES: [[@LINE+1]]:17: warning: unused return value [misrac-rule-17p7]
    while (--n) f();
    // CHECK-FIXES: {{^    }}while (--n) (void) f();

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: unused return value [misrac-rule-17p7]
    (void) (f(), (void) 0);
    // CHECK-FIXES: {{^    }}(void) ((void) f(), (void) 0);

    switch (f()) {
    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: unused return value [misrac-rule-17p7]
    case 0: f(); break;
    // CHECK-FIXES: {{^    }}case 0: (void) f(); break;
    case 1:
        // CHECK-MESSAGES: [[@LINE+1]]:9: warning: unused return value [misrac-rule-17p7]
        f();
        // CHECK-FIXES: {{^        }}(void) f();

        // CHECK-MESSAGES: [[@LINE+1]]:9: warning: unused return value [misrac-rule-17p7]
        f();
        // CHECK-FIXES: {{^        }}(void) f();
    break;
    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: unused return value [misrac-rule-17p7]
    default: f(); break;
    // CHECK-FIXES: {{^    }}default: (void) f(); break;
    }

}
