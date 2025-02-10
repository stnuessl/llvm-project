// RUN: %check_clang_tidy %s misc-no-union %t

// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: disallowed declaration of union 'x' [misc-no-union]
union x {
    char a[4];
    int num;
};
// CHECK-FIXES: {{^}};

void fn() {
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: disallowed declaration of union 'y' [misc-no-union]
    union y {
        char a[4];
        float num;
    };
    // CHECK-FIXES: {{^    }};
}
