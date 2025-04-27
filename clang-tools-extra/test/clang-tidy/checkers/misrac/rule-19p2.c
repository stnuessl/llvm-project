// RUN: %check_clang_tidy %s misrac-rule-19p2 %t

// CHECK-MESSAGES: [[@LINE+1]]:1: warning: union used [misrac-rule-19p2]
union x {
    char a[4];
    int b;
};

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: union used [misrac-rule-19p2]
typedef union {
    double a;
    int b;
} y;

void fn() {
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: union used [misrac-rule-19p2]
    union z {
        char a[4];
        float b;
    };
}
