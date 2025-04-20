// RUN: %check_clang_tidy %s misrac-rule-10p2 %t

void f1(char c1, char c2) {
    enum { E0 };
    enum e { E1 };

    (void) (c1 + 0);
    (void) (0u + c2);
    (void) (c1 - 0);
    (void) (c2 - c1);
    (void) (c2 - 0 - c1);
    (void) (c1 + E0);
    (void) (c1 + E0 + 0);

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: '+' operation uses essential types 'character' and 'character' [misrac-rule-10p2]
    (void) (c1 + c2);

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: '-' operation uses essential types 'enum' and 'character' [misrac-rule-10p2]
    (void) (E1 - '\0');

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: '+' operation uses essential types 'character' and 'floating' [misrac-rule-10p2]
    (void) (c2 + 0.0f);

    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: '-' operation uses essential types 'signed' and 'character' [misrac-rule-10p2]
    (void) (0 - (c2 - c1));

    // CHECK-MESSAGES: [[@LINE+1]]:22: warning: '+' operation uses essential types 'character' and 'character' [misrac-rule-10p2]
    (void) ('\0' + 0 + '\0');
}
