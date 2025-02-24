// RUN: %check_clang_tidy %s embedded-no-partial-array-init %t

int a1[] = {0, 0};
int a2[3] = {1, 2, 3};

// CHECK-MESSAGES: [[@LINE+1]]:13: warning: disallowed partial array initialization [embedded-no-partial-array-init]
int a3[3] = {0, 1};
// CHECK-FIXES: {{^}}int a3[3] = {0};

int a4[3] = { 0 };

// CHECK-MESSAGES: [[@LINE+1]]:13: warning: disallowed partial array initialization [embedded-no-partial-array-init]
int a5[3] = { 1 };
// CHECK-FIXES: {{^}}int a5[3] = {0};

