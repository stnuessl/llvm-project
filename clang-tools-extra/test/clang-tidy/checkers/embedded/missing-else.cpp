// RUN: %check_clang_tidy %s embedded-missing-else %t
// RUN: %check_clang_tidy -check-suffix=KR %s embedded-missing-else %t \
// RUN:     --\
// RUN:     -config='{ CheckOptions: \
// RUN:         { \
// RUN:             embedded-missing-else.IndentationStyle: 'K\&R' \
// RUN:         } \
// RUN:     }'

void f()
{
    if (1) {}

    if (1) {}
    else {}

    if (1) {}
    else if (1) {}
    else if (1) {}
    else if (1) {}
    else {}

    // CHECK-MESSAGES: [[@LINE+4]]:5: warning: 'else if' must be terminated with an 'else' statement
    // CHECK-MESSAGES-KR: [[@LINE+3]]:5: warning: 'else if' must be terminated with an 'else' statement
    if (1) {}
    else if (1) {}
    else if (1) {}
    // CHECK-FIXES: {{^    }}else
    // CHECK-FIXES: {{^    }}{
    // CHECK-FIXES: {{^        }}/* Nothing to do */
    // CHECK-FIXES: {{^    }}}
    // CHECK-FIXES-KR: {{^    }}else {
    // CHECK-FIXES-KR: {{^        }}/* Nothing to do */
    // CHECK-FIXES-KR: {{^    }}}

    // CHECK-MESSAGES: [[@LINE+4]]:5: warning: 'else if' must be terminated with an 'else' statement
    // CHECK-MESSAGES-KR: [[@LINE+3]]:5: warning: 'else if' must be terminated with an 'else' statement
    if (1) {}
    else if (1) {}
    else if (1)
    {
        // CHECK-MESSAGES: [[@LINE+3]]:9: warning: 'else if' must be terminated with an 'else' statement
        // CHECK-MESSAGES-KR: [[@LINE+2]]:9: warning: 'else if' must be terminated with an 'else' statement
        if (1) {}
        else if (1) {}
        // CHECK-FIXES: {{^        }}else
        // CHECK-FIXES: {{^        }}{
        // CHECK-FIXES: {{^            }}/* Nothing to do */
        // CHECK-FIXES: {{^        }}}
    }
    // CHECK-FIXES: {{^    }}else
    // CHECK-FIXES: {{^    }}{
    // CHECK-FIXES: {{^        }}/* Nothing to do */
    // CHECK-FIXES: {{^    }}}
    // CHECK-FIXES-KR: {{^    }}else {
    // CHECK-FIXES-KR: {{^        }}/* Nothing to do */
    // CHECK-FIXES-KR: {{^    }}}
}
