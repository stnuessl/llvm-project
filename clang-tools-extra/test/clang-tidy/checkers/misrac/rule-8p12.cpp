// RUN: %check_clang_tidy %s misrac-rule-8p12 %t

enum e1 { e11 = 3, e12, e13 = 5, e14 = 5 };

// CHECK-MESSAGES: [[@LINE+1]]:11: warning: non-unique implicitly defined enumeration value [misrac-rule-8p12]
enum e2 { e21, e22 = 0 };
// CHECK-FIXES: {{^}}enum e2 { e21 = 0, e22 = 0 };

// CHECK-MESSAGES: [[@LINE+1]]:25: warning: non-unique implicitly defined enumeration value [misrac-rule-8p12]
enum e3 { e31 = 3, e32, e33, e34 = 5 };
// CHECK-FIXES: {{^}}enum e3 { e31 = 3, e32, e33 = 5, e34 = 5 };

// CHECK-MESSAGES: [[@LINE+1]]:34: warning: non-unique implicitly defined enumeration value [misrac-rule-8p12]
enum e4 { e41 = 0, e42, e43 = 0, e44};
// CHECK-FIXES: {{^}}enum e4 { e41 = 0, e42, e43 = 0, e44 = 1};


