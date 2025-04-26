// RUN: %check_clang_tidy %s misrac-rule-10p8 %t

enum e1 { E1_0, E1_1 };
enum e2 { E2_0 };

void g(double d);

void f(float f32) {

    (void) ((float) (f32 + 0.0f));

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast of composite expression from 'floating' to 'unsigned' [misrac-rule-10p8]
    (void) ((unsigned int) (f32 + 0.0f) + 0.0f);

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast of composite expression from 'unsigned' to 'enum' [misrac-rule-10p8]
    (void) ((enum e2) ((enum e1) f32 - E1_0));

    // CHECK-MESSAGES: [[@LINE+1]]:13: warning: essential type category cast of 32-bit composite expression to wider 64-bit type [misrac-rule-10p8]
    (void) ((double) (1.0f / (1.0f + f32)) + 1.0);
}
