// RUN: %check_clang_tidy %s misrac-rule-11p9 %t
// RUN: %check_clang_tidy -check-suffix=CONF %s misrac-rule-11p9 %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-11p9.AllowedNullPointerConstants: 'NULL_PTR' \
// RUN:         } \
// RUN:     }'

// Increase test depth by intentionally not using '(void *) 0'.
#define NULL 0
#define X_NULL ((void *) 0)

void g(int *p);

void f(void) {
    int *p = NULL;
    void (*f)(void) = X_NULL;

    p = X_NULL;
    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: forbidden integer null pointer constant [misrac-rule-11p9]
    p = 0;
    // CHECK-FIXES: {{^    }}p = NULL;
    // CHECK-FIXES-CONF: {{^    }}p = NULL_PTR;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: forbidden integer null pointer constant [misrac-rule-11p9]
    p = (int *) 0;
    // CHECK-FIXES: {{^    }}p = NULL;
    // CHECK-FIXES-CONF: {{^    }}p = NULL_PTR;

    g(NULL);
    g((void *) 0);
    g(p);

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: forbidden integer null pointer constant [misrac-rule-11p9]
    g(0);
    // CHECK-FIXES: {{^    }}g(NULL);
    // CHECK-FIXES-CONF: {{^    }}g(NULL_PTR);

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: forbidden integer null pointer constant [misrac-rule-11p9]
    g((int *) 0);
    // CHECK-FIXES: {{^    }}g(NULL);
    // CHECK-FIXES-CONF: {{^    }}g(NULL_PTR);
}

