// RUN: %check_clang_tidy %s embedded-no-early-return %t

int fn1()
{
    // CHECK-MESSAGES: [[@LINE+2]]:9: warning: disallowed early return statement [embedded-no-early-return]
    if (1)
        return 0;

    return 0;
}

int fn2()
{
    // CHECK-MESSAGES: [[@LINE+2]]:9: warning: disallowed early return statement [embedded-no-early-return]
    if (1)
        return 0;

    // CHECK-MESSAGES: [[@LINE+3]]:9: warning: disallowed early return statement [embedded-no-early-return]
    // CHECK-MESSAGES: [[@LINE+4]]:9: warning: disallowed early return statement [embedded-no-early-return]
    if (1)
        return 0;
    else
        return 0;

    // CHECK-MESSAGES: [[@LINE+1]]:1: warning: return statement missing at end of function [embedded-no-early-return]
}

void fn3() {
}

void fn4() {
    return;
}
