// RUN: %check_clang_tidy %s misrac-rule-17p12 %t

void f();

void g()
{
    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: missing address operator [misrac-rule-17p12]
    void (*fn)() = f;
    // CHECK-FIXES: {{^    }}void (*fn)() = &f;

    fn = &f;

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: missing address operator [misrac-rule-17p12]
    fn = f;
    // CHECK-FIXES: {{^    }}fn = &f;

    (void) &f;

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: missing address operator [misrac-rule-17p12]
    (void) f;
    // CHECK-FIXES: {{^    }}(void) &f;

    f();
}
