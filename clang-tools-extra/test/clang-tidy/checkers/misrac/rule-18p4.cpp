// RUN: %check_clang_tidy %s misrac-rule-18p4 %t

void f(int *p, int n) {
    p[0] = 0;
    *p = 0;

    ++p;
    p--;

    for (int i = 0; i < n; ++i)
        p[i] = 0;

    (void) (p - (const int *) 0);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: pointer used in '+=' operation [misrac-rule-18p4]
    p += 1;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: pointer used in '+' operation [misrac-rule-18p4]
    p = p + 1;

    // CHECK-MESSAGES: [[@LINE+1]]:7: warning: pointer used in '+' operation [misrac-rule-18p4]
    *(p + 0) = 0;

    // CHECK-MESSAGES: [[@LINE+1]]:11: warning: pointer used in '+' operation [misrac-rule-18p4]
    *(0 + p) = 0;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: pointer used in '-=' operation [misrac-rule-18p4]
    p -= 1;

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: pointer used in '-' operation [misrac-rule-18p4]
    p = p - 1;
}

