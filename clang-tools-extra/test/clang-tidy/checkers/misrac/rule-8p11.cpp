// RUN: %check_clang_tidy %s misrac-rule-8p11 %t

int a1[1];
int a2[] = { 0 };
extern int a3[1];
static int a4[1];

// CHECK-MESSAGES: [[@LINE+1]]:12: warning: missing size specification [misrac-rule-8p11]
extern int a5[];


