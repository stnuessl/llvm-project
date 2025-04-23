// RUN: %check_clang_tidy %s misrac-rule-10p5 %t

enum e1 { E1_0 };
enum e2 { E2_0 };

void f1(void) {
    (void) ((_Bool) 0);
    (void) ((_Bool) 0u);
    (void) ((_Bool) 1);
    (void) ((_Bool) 1u);
    (void) ((_Bool) (0 == 0));

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'enum' to 'boolean' [misrac-rule-10p5]
    (void) ((_Bool) E1_0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'character' to 'boolean' [misrac-rule-10p5]
    (void) ((_Bool) '\0');

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'signed' to 'boolean' [misrac-rule-10p5]
    (void) ((_Bool) 2);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'unsigned' to 'boolean' [misrac-rule-10p5]
    (void) ((_Bool) 2u);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'floating' to 'boolean' [misrac-rule-10p5]
    (void) ((_Bool) 0.0);
}

void f2(void) {
    (void) ((char) '\0');
    (void) ((char) E1_0);
    (void) ((char) 0);
    (void) ((char) 0u);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'boolean' to 'character' [misrac-rule-10p5]
    (void) ((char) (_Bool) 0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'floating' to 'character' [misrac-rule-10p5]
    (void) ((char) 0.0);
}

void f3(void) {
    (void) ((enum e1) E1_0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'boolean' to 'enum' [misrac-rule-10p5]
    (void) ((enum e1) (0 != 0));

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'character' to 'enum' [misrac-rule-10p5]
    (void) ((enum e1) '\0');

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast to different enumeration type [misrac-rule-10p5]
    (void) ((enum e1) E2_0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'signed' to 'enum' [misrac-rule-10p5]
    (void) ((enum e1) 0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'unsigned' to 'enum' [misrac-rule-10p5]
    (void) ((enum e1) 0u);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'floating' to 'enum' [misrac-rule-10p5]
    (void) ((enum e1) 0.0);
}

void f4(void) {
    (void) ((int) '\0');
    (void) ((int) E1_0);
    (void) ((int) 0);
    (void) ((int) 0u);
    (void) ((int) 0.0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'boolean' to 'signed' [misrac-rule-10p5]
    (void) ((int) (0 > 0));
}

void f5(void) {
    (void) ((unsigned int) '\0');
    (void) ((unsigned int) E2_0);
    (void) ((unsigned int) 0);
    (void) ((unsigned int) 0u);
    (void) ((unsigned int) 0.0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'boolean' to 'unsigned' [misrac-rule-10p5]
    (void) ((unsigned int) (0 > 0));
}

void f6(void) {
    (void) ((double) E2_0);
    (void) ((double) 0);
    (void) ((double) 0u);
    (void) ((double) 0.0f);
    (void) ((float) 0.0);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'boolean' to 'floating' [misrac-rule-10p5]
    (void) ((double) (0 < 0));

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast from 'character' to 'floating' [misrac-rule-10p5]
    (void) ((double) '\0');
}

void f7(void) {
    (void) (void *) 0;
    (void) (unsigned long *) 0ul;
}
