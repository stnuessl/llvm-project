// RUN: %check_clang_tidy %s misrac-rule-8p8 %t

extern int v1;
extern int v1;
int v1;

// CHECK-MESSAGES: [[@LINE+2]]:12: warning: 'static' missing from declaration with internal linkage [misrac-rule-8p8]
static int v2;
extern int v2;
// CHECK-FIXES: {{^$}}

void f1();
void f1();
void f1() {}

static void f2();
static void f2();
static void f2() {}

// CHECK-MESSAGES: [[@LINE+2]]:6: warning: 'static' missing from declaration with internal linkage [misrac-rule-8p8]
static void f3();
void f3() {}
// CHECK-FIXES: {{^}}static void f3() {}

// CHECK-MESSAGES: [[@LINE+3]]:6: warning: 'static' missing from declaration with internal linkage [misrac-rule-8p8]
// CHECK-MESSAGES: [[@LINE+4]]:6: warning: 'static' missing from declaration with internal linkage [misrac-rule-8p8]
static void f4();
void f4();
// CHECK-FIXES: {{^}}static void f4();
void f4() {}
// CHECK-FIXES: {{^}}static void f4() {}

// CHECK-MESSAGES: [[@LINE+3]]:13: warning: 'static' missing from declaration with internal linkage [misrac-rule-8p8]
// CHECK-MESSAGES: [[@LINE+4]]:6: warning: 'static' missing from declaration with internal linkage [misrac-rule-8p8]
static void f5();
extern void f5();
// CHECK-FIXES: {{^$}}
void f5() {}
// CHECK-FIXES: {{^}}static void f5() {}
