// RUN: %check_clang_tidy %s misrac-rule-15p2 %t

void f(int n) {

l1:
    if (n < 1)
        goto out;

// CHECK-MESSAGES: [[@LINE+2]]:14: warning: backward jumping goto [misrac-rule-15p2]
// CHECK-MESSAGES: [[@LINE+1]]:1: note: label defined here
l2: if (--n) goto l2;

// CHECK-MESSAGES: [[@LINE+3]]:9: warning: backward jumping goto [misrac-rule-15p2]
// CHECK-MESSAGES: [[@LINE-9]]:1: note: label defined here
    if (!n)
        goto l1;

out:
    ;
}

