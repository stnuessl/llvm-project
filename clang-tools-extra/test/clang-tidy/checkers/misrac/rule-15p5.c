// RUN: %check_clang_tidy %s misrac-rule-15p5 %t

int fn1()
{
    // CHECK-MESSAGES: [[@LINE+2]]:9: warning: early return statement [misrac-rule-15p5]
    if (1)
        return 0;

    return 0;
}

int fn2()
{
    // CHECK-MESSAGES: [[@LINE+2]]:9: warning: early return statement [misrac-rule-15p5]
    if (1)
        return 0;

    // CHECK-MESSAGES: [[@LINE+3]]:9: warning: early return statement [misrac-rule-15p5]
    // CHECK-MESSAGES: [[@LINE+4]]:9: warning: early return statement [misrac-rule-15p5]
    if (1)
        return 0;
    else
        return 0;

    return 0;
}

void fn3() {
}

void fn4() {
    return;
}

void fn5() {
    // CHECK-MESSAGES: [[@LINE+2]]:9: warning: early return statement [misrac-rule-15p5]
    if (1)
        return;

}
