// RUN: %check_clang_tidy %s misrac-rule-13p6 %t

int g();

void f1(int val) {
    (void) sizeof(0);
    (void) sizeof(int);
    (void) sizeof(val);

    (void) sizeof(val + 1);

    // CHECK-MESSAGES: [[@LINE+1]]:19: warning:
    (void) sizeof(++val);

    // CHECK-MESSAGES: [[@LINE+1]]:23: warning:
    (void) sizeof(val += 1);

    // CHECK-MESSAGES: [[@LINE+1]]:19: warning:
    (void) sizeof(g());
}

void f2(volatile int *val) {
    (void) sizeof(val);
    (void) sizeof(*val);

    // CHECK-MESSAGES: [[@LINE+1]]:19: warning:
    (void) sizeof(*val++);
}

void f3(int n, volatile int v) {

    // CHECK-MESSAGES: [[@LINE+1]]:19: warning:
    (void) sizeof(void (*[n])(int a[v]));
}
