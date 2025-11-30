// RUN: %check_clang_tidy %s misrac-rule-18p5 %t
// RUN: %check_clang_tidy -check-suffix=CONF %s misrac-rule-18p5 %t \
// RUN:     -- \
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-18p5.MaxNestingLevel: 3 \
// RUN:         } \
// RUN:     }'

void f1() {
    typedef int *intp;
    typedef int **intpp;
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
    typedef int ***intppp;

    int **p1;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
    int ***p2;

    // CHECK-MESSAGES: [[@LINE+2]]:5: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
    // CHECK-MESSAGES-CONF: [[@LINE+1]]:5: warning: declaration execeeds pointer nesting level of 3 [misrac-rule-18p5]
    int ****p3;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
    intp **p4;

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
    intpp *p5;

    int **(**fn1)(void);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
    int **(***fn2)(void);

}

void f2() {
    struct x {
        int **p1;
        // CHECK-MESSAGES: [[@LINE+1]]:9: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
        int ***p2;
    };

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: declaration execeeds pointer nesting level of 2 [misrac-rule-18p5]
    struct x ***p1;
}

