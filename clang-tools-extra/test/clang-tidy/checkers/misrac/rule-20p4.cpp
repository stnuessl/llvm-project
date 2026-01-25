// RUN: %check_clang_tidy %s misrac-rule-20p4 %t

#define nok 0
#define ok 1

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: macro defined with same name as keyword 'int' [misrac-rule-20p4]
#define int int32

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: macro defined with same name as keyword 'private' [misrac-rule-20p4]
#define private public

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: macro defined with same name as keyword 'inline' [misrac-rule-20p4]
#define inline

#if nok
#define private public
#endif

// CHECK-MESSAGES: [[@LINE+2]]:9: warning: macro defined with same name as keyword 'static' [misrac-rule-20p4]
#if ok
#define static
#endif

// CHECK-MESSAGES: [[@LINE+1]]:9: warning: macro defined with same name as keyword 'while' [misrac-rule-20p4]
#define while(x) for (;(x);)

