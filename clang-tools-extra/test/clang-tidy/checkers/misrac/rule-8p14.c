// RUN: %check_clang_tidy %s misrac-rule-8p14 %t

// CHECK-MESSAGES: [[@LINE+1]]:14: warning: 'restrict' qualifier used [misrac-rule-8p14]
typedef int *restrict iptr;
// CHECK-FIXES: {{^}}typedef int *iptr;

// CHECK-MESSAGES: [[@LINE+1]]:13: warning: 'restrict' qualifier used [misrac-rule-8p14]
void f(int *restrict p) {
// CHECK-FIXES: {{^}}void f(int *p) {

    // CHECK-MESSAGES: [[@LINE+1]]:11: warning: 'restrict' qualifier used [misrac-rule-8p14]
    char *__restrict q = (void *) 0;
    // CHECK-FIXES: {{^    }}char *q = (void *) 0;
}
