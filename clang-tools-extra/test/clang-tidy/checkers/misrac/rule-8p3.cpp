// RUN: %check_clang_tidy %s misrac-rule-8p3 %t

typedef int s32;

extern s32 v1;
s32 v1;

// CHECK-MESSAGES: [[@LINE+3]]:1: warning: type mismatch with primary declaration [misrac-rule-8p3]
// CHECK-MESSAGES: [[@LINE+1]]:8: note: declaration here uses 's32' (aka 'int') and not 'int'
extern s32 v2;
int v2;
// CHECK-FIXES: {{^}}s32 v2;

// CHECK-MESSAGES: [[@LINE+4]]:1: warning: type mismatch with primary declaration [misrac-rule-8p3]
// CHECK-MESSAGES: [[@LINE+1]]:1: note: declaration here uses 'int' and not 's32' (aka 'int')
int f1();
int f1();
s32 f1() {}
// CHECK-FIXES: {{^}}int f1() {}

// CHECK-MESSAGES: [[@LINE+3]]:9: warning: type mismatch with primary declaration [misrac-rule-8p3]
// CHECK-MESSAGES: [[@LINE+1]]:9: note: declaration here uses 's32' (aka 'int') and not 'int'
void f2(s32, const int *b);
void f2(int a, const int *b) {}
// CHECK-FIXES: {{^}}void f2(s32 a, const int *b) {}

