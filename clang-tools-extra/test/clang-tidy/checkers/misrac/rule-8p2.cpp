// RUN: %check_clang_tidy %s misrac-rule-8p2 %t

void f1();

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: disallowed unnamed parameter [misrac-rule-8p2]
void f2(int);

// CHECK-MESSAGES: [[@LINE+2]]:9: warning: disallowed unnamed parameter [misrac-rule-8p2]
// CHECK-MESSAGES: [[@LINE+1]]:17: warning: disallowed unnamed parameter [misrac-rule-8p2]
void f3(void *, long);

void f4(void);
