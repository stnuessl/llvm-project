// RUN: %check_clang_tidy %s misrac-rule-9p3 %t

int a1[] = {0, 0};
int a2[3] = {1, 2, 3};

// CHECK-MESSAGES: [[@LINE+2]]:13: warning: partial array initialization [misrac-rule-9p3]
// CHECK-MESSAGES: [[@LINE+1]]:13: note: 2 out of 3 elements initialized
int a3[3] = {0, 1};

int a4[3] = { 0 };

// CHECK-MESSAGES: [[@LINE+2]]:13: warning: partial array initialization [misrac-rule-9p3]
// CHECK-MESSAGES: [[@LINE+1]]:13: note: 1 out of 3 elements initialized
int a5[3] = { 1 };

