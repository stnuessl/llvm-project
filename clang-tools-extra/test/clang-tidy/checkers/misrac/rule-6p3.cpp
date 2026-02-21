// RUN: %check_clang_tidy %s misrac-rule-6p3 %t

union u1 {
    void *p;
    int n;
};

union u2 {
    // CHECK-MESSAGES: [[@LINE+2]]:9: warning: bit field 'n' declared as member of a union [misrac-rule-6p3]
    void *p;
    int n : 8;
};

struct s1 {
    int x : 8;
    int y : 8;
};

struct s2 {
    // CHECK-MESSAGES: [[@LINE+3]]:13: warning: bit field 'n' declared as member of a union [misrac-rule-6p3]
    union {
        void *p;
        int n : 4;
    } u;
};

