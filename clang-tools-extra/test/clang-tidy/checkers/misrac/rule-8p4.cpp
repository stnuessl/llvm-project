// RUN: %check_clang_tidy %s misrac-rule-8p4 %t
// RUN: %check_clang_tidy -check-suffix=CONF %s misrac-rule-8p4 %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             misrac-rule-8p4.IgnoreFunctions: "^::f3$" \
// RUN:         } \
// RUN:     }'


extern int v1;

int v1;

// CHECK-MESSAGES: [[@LINE+2]]:5: warning: missing declaration for variable definition [misrac-rule-8p4]
// CHECK-MESSAGES-CONF: [[@LINE+1]]:5: warning: missing declaration for variable definition [misrac-rule-8p4]
int v2;
// CHECK-FIXES: {{^}}static int v2;
// CHECK-FIXES-CONF: {{^}}static int v2;

static int v3;

void f1(int i);

static void f2(void) {}

void f1(int i) {}

// CHECK-MESSAGES: [[@LINE+1]]:6: warning: missing declaration for function definition [misrac-rule-8p4]
void f3(void) {}
// CHECK-FIXES: {{^}}static void f3(void) {}

