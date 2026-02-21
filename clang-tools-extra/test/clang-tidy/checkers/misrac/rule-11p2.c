// RUN: %check_clang_tidy %s misrac-rule-11p2 %t

struct s1;
struct s2;

void f(struct s1 *s1, struct s2 *s2) {
    void *p;
    int n;

    n = (int) 0.0f;
    s1 = (void *) 0;
    s1 = (struct s1 *) 0;
    p = (void *) &n;

    (void) s1;
    (void) s2;


    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: cast involving pointer to incomplete type 'struct s1' [misrac-rule-11p2]
    s1 = (struct s1 *) &n;

    // CHECK-MESSAGES: [[@LINE+1]]:10: warning: cast involving pointer to incomplete type 'struct s1' [misrac-rule-11p2]
    s1 = (struct s2 *) 0;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: cast involving pointer to incomplete type 'struct s2' [misrac-rule-11p2]
    p = (struct s2 *) 0;
}

