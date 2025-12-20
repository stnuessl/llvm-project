// RUN: %check_clang_tidy %s misrac-rule-8p3 %t

void f1(int a);
void f1(signed a) {}

// CHECK-MESSAGES: [[@LINE+3]]:9: warning: type mismatch with primary declaration [misrac-rule-8p3]
// CHECK-MESSAGES: [[@LINE+1]]:9: note: declaration here uses 'int *const' and not 'int *'
void f2(int *const a);
void f2(int *a) {}
// CHECK-FIXES: {{^}}void f2(int *const a) {}

