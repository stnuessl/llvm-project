// RUN: %check_clang_tidy %s misrac-rule-11p9 %t

#define NULL 0

void g(int *p);

void f() {
    int *p;

    p = nullptr;
    p = NULL;
    p = static_cast<int *>(0);

    g(p);
    g(NULL);
    g(nullptr);

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: forbidden integer null pointer constant [misrac-rule-11p9]
    p = 0;
    // CHECK-FIXES: {{^    }}p = nullptr;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: forbidden integer null pointer constant [misrac-rule-11p9]
    g(0);
    // CHECK-FIXES: {{^    }}g(nullptr);
}
