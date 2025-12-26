// RUN: %check_clang_tidy %s misrac-rule-7p3 %t

#define block0 (0u)
#define block1 (0L + 1l)

long a1 = 0L;

// CHECK-MESSAGES: [[@LINE+1]]:11: warning: disallowed use of lowercase 'l' in literal suffix [misrac-rule-7p3]
long a2 = 0l;

unsigned long a3 = 0uL;
unsigned long a4 = 0x0UL;

// CHECK-MESSAGES: [[@LINE+1]]:20: warning: disallowed use of lowercase 'l' in literal suffix [misrac-rule-7p3]
unsigned long a5 = 0ul;

// CHECK-MESSAGES: [[@LINE+1]]:25: warning: disallowed use of lowercase 'l' in literal suffix [misrac-rule-7p3]
unsigned long long a6 = 0ull;

unsigned int a7 = block0;

// CHECK-MESSAGES: [[@LINE+1]]:20: warning: disallowed use of lowercase 'l' in literal suffix [misrac-rule-7p3]
unsigned long a8 = block1;
