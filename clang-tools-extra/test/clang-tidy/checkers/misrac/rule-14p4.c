// RUN: %check_clang_tidy %s misrac-rule-14p4 %t

#define FALSE ((_Bool) 0)
#define TRUE ((_Bool) 1)

_Bool g(void);
int h(void);

void f(void *p) {
    _Bool b;
    int i;

    if (p != (void *) 0) {}
    if (TRUE) {}
    if (!TRUE) {}
    while (TRUE) {}
    while (1) {}
    while (0 == FALSE && 1 == TRUE) {}
    for (;;) {}
    for (int i = 0; FALSE; ++i) {}
    do { } while (0);
    do { } while (0u);
    if (g()) {}
    if (b = g()) {}
    if ((i = h()) != 0) {}

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: inappropriate essential type 'undefined' for condition [misrac-rule-14p4]
    if (p) { }

    // CHECK-MESSAGES: [[@LINE+2]]:14: warning: inappropriate essential type 'signed' for condition [misrac-rule-14p4]
    if (FALSE) {}
    else if (0) {}

    // CHECK-MESSAGES: [[@LINE+1]]:30: warning: inappropriate essential type 'unsigned' for condition [misrac-rule-14p4]
    for (unsigned int i = 1; i; --i) {}

    // CHECK-MESSAGES: [[@LINE+1]]:9: warning: inappropriate essential type 'signed' for condition [misrac-rule-14p4]
    if (h()) {}

    // CHECK-MESSAGES: [[@LINE+1]]:11: warning: inappropriate essential type 'signed' for condition [misrac-rule-14p4]
    if (i = h()) {}
}

