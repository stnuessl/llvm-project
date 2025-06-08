// RUN: %check_clang_tidy %s misrac-rule-15p7 %t

void f(void)
{
    if (1) {}

    if (1) {}
    else {}

    if (1) {}
    else if (1) {}
    else if (1) {}
    else if (1) {}
    else {}

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: 'else if' not terminated by 'else' [misrac-rule-15p7]
    if (1) {}
    else if (1) {}
    else if (1) {}

    // CHECK-MESSAGES: [[@LINE+3]]:5: warning: 'else if' not terminated by 'else' [misrac-rule-15p7]
    if (1) {}
    else if (1) {}
    else if (1)
    {
        // CHECK-MESSAGES: [[@LINE+2]]:9: warning: 'else if' not terminated by 'else' [misrac-rule-15p7]
        if (1) {}
        else if (1) {}
    }
}

