// RUN: %check_clang_tidy %s misrac-rule-8p10 %t

void f1() {}

// CHECK-MESSAGES: [[@LINE+1]]:13: warning: 'static' storage classifier required in inline function declaration [misrac-rule-8p10]
inline void f2() {}
// CHECK-FIXES: {{^static}} inline void f2() {}

static void f3() {}

static inline void f4() {}

class c {
public:
    inline void f() const {}
};
