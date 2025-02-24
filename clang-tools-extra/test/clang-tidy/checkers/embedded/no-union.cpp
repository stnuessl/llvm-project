// RUN: %check_clang_tidy %s embedded-no-union %t

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: disallowed declaration of union [embedded-no-union]
union x {
    char a[4];
    int b;
};

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: disallowed declaration of union [embedded-no-union]
typedef union {
    double a;
    int b;
} y;

void fn() {
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: disallowed declaration of union [embedded-no-union]
    union z {
        char a[4];
        float b;
    };
}

