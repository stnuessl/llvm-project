// RUN: %check_clang_tidy %s misrac-rule-7p1 %t

#define ZERO 00

int a1 = 0;
int a2 = 0u;
int a3 = 0U;
int a4 = 0x00;

// CHECK-MESSAGES: [[@LINE+1]]:10: warning: octal constant used [misrac-rule-7p1]
int a5 = 00;
// CHECK-FIXES: {{^}}int a5 = 0x0;

// CHECK-MESSAGES: [[@LINE+1]]:10: warning: octal constant used [misrac-rule-7p1]
int a6 = 0777;
// CHECK-FIXES: {{^}}int a6 = 0x1ff;

// CHECK-MESSAGES: [[@LINE+1]]:10: warning: octal constant used [misrac-rule-7p1]
int a7 = ZERO;
