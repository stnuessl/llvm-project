// RUN: %check_clang_tidy %s misrac-rule-17p8 %t
// RUN: %check_clang_tidy -check-suffix=IGNORED %s misrac-rule-17p8 %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-17p8.AddrOpDiagLevel: "Ignored" \
// RUN:         } \
// RUN:     }'
// RUN: %check_clang_tidy -check-suffix=REMARK %s misrac-rule-17p8 %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-17p8.AddrOpDiagLevel: "Remark" \
// RUN:         } \
// RUN:     }'

void f1(int val, int *ptr) {
    // CHECK-MESSAGES: [[@LINE+2]]:22: warning: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:22: remark: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    const int *p1 = (&val);

    // CHECK-MESSAGES: [[@LINE+2]]:16: warning: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:16: remark: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    int *p2 = (&val);

    p1 = &val;
    p1 = (const int *) &val;

    *ptr = 0;
    ptr[0] = 0;

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:5: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:5: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    val = 0;

    // CHECK-MESSAGES: [[@LINE+3]]:6: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:6: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:6: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    (val) = 0;

    // CHECK-MESSAGES: [[@LINE+3]]:7: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:7: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:7: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    ++val;

    // CHECK-MESSAGES: [[@LINE+3]]:6: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:6: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:6: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    (val)--;

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:5: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:5: warning: forbidden modification of parameter 'val' [misrac-rule-17p8]
    val += 0;

    // CHECK-MESSAGES: [[@LINE+2]]:10: warning: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:10: remark: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    p2 = &val;

    // CHECK-MESSAGES: [[@LINE+2]]:18: warning: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:18: remark: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    p2 = (int *) &val;

    // CHECK-MESSAGES: [[@LINE+2]]:7: warning: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:7: remark: use of address operator may lead to unintentional modification of parameter 'val' [misrac-rule-17p8]
    *(&val) = 0;

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: forbidden modification of parameter 'ptr' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:5: warning: forbidden modification of parameter 'ptr' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:5: warning: forbidden modification of parameter 'ptr' [misrac-rule-17p8]
    ptr = (void *) 0;

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: forbidden modification of parameter 'ptr' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:5: warning: forbidden modification of parameter 'ptr' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:5: warning: forbidden modification of parameter 'ptr' [misrac-rule-17p8]
    ptr++;
}

struct s1 {
    int x;
    int *p;
};

struct s2 {
    struct s1 *s1;
};

typedef struct s1 *s1_ptr;

void f2(struct s1 val1, struct s2 val2, struct s1 *ptr1, s1_ptr ptr2) {
    const int *p1 = (const int *) &val1.x;

    // CHECK-MESSAGES: [[@LINE+2]]:29: warning: use of address operator may lead to unintentional modification of parameter 'val1' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:29: remark: use of address operator may lead to unintentional modification of parameter 'val1' [misrac-rule-17p8]
    const int *p2 = (int *) &val1.x;

    *val1.p = 0;
    val2.s1->x = 0;
    (*val2.s1).x = 0;

    ptr1->x = 0;
    ptr1->p = (void *) 0;
    ptr2->x = 0;

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: forbidden modification of parameter 'val1' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:5: warning: forbidden modification of parameter 'val1' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:5: warning: forbidden modification of parameter 'val1' [misrac-rule-17p8]
    val1.x = 5;

    // CHECK-MESSAGES: [[@LINE+3]]:7: warning: forbidden modification of parameter 'val1' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:7: warning: forbidden modification of parameter 'val1' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:7: warning: forbidden modification of parameter 'val1' [misrac-rule-17p8]
    --val1.x;

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: forbidden modification of parameter 'val2' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:5: warning: forbidden modification of parameter 'val2' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:5: warning: forbidden modification of parameter 'val2' [misrac-rule-17p8]
    val2.s1 = (void *) 0;

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: forbidden modification of parameter 'ptr1' [misrac-rule-17p8]
    // CHECK-MESSAGES-IGNORED: [[@LINE+2]]:5: warning: forbidden modification of parameter 'ptr1' [misrac-rule-17p8]
    // CHECK-MESSAGES-REMARK: [[@LINE+1]]:5: warning: forbidden modification of parameter 'ptr1' [misrac-rule-17p8]
    ptr1 = (void *) 0;
}

void f3(struct s2 *values, int index) {
    values[0].s1[index].x = 0;
    (*values).s1[index].x = 0;
    values->s1[index].x = 0;
}

