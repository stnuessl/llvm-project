// RUN: %check_clang_tidy %s misrac-rule-15p6 %t

void f() {

    // CHECK-MESSAGES: [[@LINE+1]]:12: warning: missing compound statement [misrac-rule-15p6]
    if (1) ;

    // CHECK-MESSAGES: [[@LINE+2]]:12: warning: missing compound statement [misrac-rule-15p6]
    // CHECK-MESSAGES: [[@LINE+2]]:10: warning: missing compound statement [misrac-rule-15p6]
    if (1) ;
    else ;

    // CHECK-MESSAGES: [[@LINE+1]]:8: warning: missing compound statement [misrac-rule-15p6]
    do ; while (0);

    // CHECK-MESSAGES: [[@LINE+1]]:15: warning: missing compound statement [misrac-rule-15p6]
    while (1) if (1) { ; }

    // CHECK-MESSAGES: [[@LINE+2]]:15: warning: missing compound statement [misrac-rule-15p6]
    // CHECK-MESSAGES: [[@LINE+1]]:22: warning: missing compound statement [misrac-rule-15p6]
    while (1) if (1) ;

    // CHECK-MESSAGES: [[@LINE+1]]:14: warning: missing compound statement [misrac-rule-15p6]
    for (;;) ;

    // CHECK-MESSAGES: [[@LINE+1]]:16: warning: missing compound statement [misrac-rule-15p6]
    switch (0) case 0: break;
}

