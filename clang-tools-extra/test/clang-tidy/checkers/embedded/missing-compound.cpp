// RUN: %check_clang_tidy %s embedded-missing-compound %t

void f() {

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: the then branch of an 'if condition' must be a compound statement [embedded-missing-compound]
    if (1) ;
    // CHECK-FIXES: {{^    }}if (1) { ; }

    // CHECK-MESSAGES: [[@LINE+2]]:12: warning: the then branch of an 'if condition' must be a compound statement [embedded-missing-compound]
    // CHECK-MESSAGES: [[@LINE+2]]:10: warning: the else branch of an 'if condition' must be a compound statement [embedded-missing-compound]
    if (1) ;
    else ;
    // CHECK-FIXES: {{^    }}if (1) { ; }
    // CHECK-FIXES: {{^    }}else { ; }

    // CHECK-MESSAGES: [[@LINE+1]]:8: warning: the body of a 'do while loop' must be a compound statement [embedded-missing-compound]
    do ; while (0);
    // CHECK-FIXES: {{^    }}do { ; } while (0);

    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: the body of a 'while loop' must be a compound statement [embedded-missing-compound]
    while (1) if (1) { ; }

    // CHECK-MESSAGES: [[@LINE+2]]:15: warning: the body of a 'while loop' must be a compound statement [embedded-missing-compound]
    // CHECK-MESSAGES: [[@LINE+1]]:22: warning: the then branch of an 'if condition' must be a compound statement [embedded-missing-compound]
    while (1) if (1) ;
    // CHECK-FIXES: {{^    }}while (1) if (1) { ; }

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: the body of a 'for loop' must be a compound statement [embedded-missing-compound]
    for (;;) ;
    // CHECK-FIXES: {{^    }}for (;;) { ; }

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: the body of a 'switch' must be a compound statement [embedded-missing-compound]
    switch (0) case 0: break;
    // CHECK-FIXES: {{^    }}switch (0) { case 0: break; }
}

