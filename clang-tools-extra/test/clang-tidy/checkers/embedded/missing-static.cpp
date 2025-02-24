// RUN: %check_clang_tidy %s embedded-missing-static %t

void f1() {}

// CHECK-MESSAGES: [[@LINE+1]]:13: warning: 'static' storage classifier missing from inline function declaration 'f2' [embedded-missing-static]
inline void f2() {}
// CHECK-FIXES: {{^static}} inline void f2() {}

static void f3() {}

static inline void f4() {}

