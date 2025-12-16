// RUN: %check_clang_tidy %s misrac-rule-15p1 %t

void f(int n) {
    if (n) {
        // CHECK-MESSAGES: [[@LINE+1]]:9: warning: 'goto' should not be used [misrac-rule-15p1]
        goto out;
    }

again:
    switch (n) {
    case 0:
        --n;
        // CHECK-MESSAGES: [[@LINE+1]]:9: warning: 'goto' should not be used [misrac-rule-15p1]
        goto again;
    default:
        break;
    }

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: 'goto' should not be used [misrac-rule-15p1]
    goto out;

out:
    ;
}

