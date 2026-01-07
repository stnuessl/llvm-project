// RUN: %check_clang_tidy %s misrac-rule-12p1 %t

void f1(int a, int b) {
    int x = sizeof(int);

    x = (a == b) ? a : (a - b);
    x = x << (b + 1);

    if (a && b && x) {}
    if (a && ((a + b) & 0x01) && x) {}

    // CHECK-MESSAGES: [[@LINE+2]]:11: warning: implicit operator precedence [misrac-rule-12p1]
    // CHECK-MESSAGES: [[@LINE+1]]:24: warning: implicit operator precedence [misrac-rule-12p1]
    x = a == b ? a : a - b;
    // CHECK-FIXES: {{^    }}x = (a == b) ? a : (a - b);

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: implicit operator precedence [misrac-rule-12p1]
    x = x << b + 1;
    // CHECK-FIXES: {{^    }}x = x << (b + 1);

    // CHECK-MESSAGES: [[@LINE+2]]:16: warning: implicit operator precedence [misrac-rule-12p1]
    // CHECK-MESSAGES: [[@LINE+1]]:20: warning: implicit operator precedence [misrac-rule-12p1]
    if (a && a + b & 0x01 && x) {}
    // CHECK-FIXES: {{^    }}if (a && ((a + b) & 0x01) && x) {}

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: missing parentheses [misrac-rule-12p1]
    x = sizeof x + a;
    // CHECK-FIXES: {{^    }}x = sizeof(x) + a;

    // CHECK-MESSAGES: [[@LINE+1]]:18: warning: implicit operator precedence [misrac-rule-12p1]
    x += (int) b % 10 + '0';
    // CHECK-FIXES: {{^    }}x += ((int) b % 10) + '0';
}

void f2(int *a, int n) {
    while (n--)
        *a++ = 0;
}

