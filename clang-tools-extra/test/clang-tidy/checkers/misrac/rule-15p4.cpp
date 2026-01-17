// RUN: %check_clang_tidy %s misrac-rule-15p4 %t

void f1(int n) {
    do {
        switch (n) {
        case 0:
            goto out;
        default:
            break;
        }
    } while (1);

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: loop contains more than one secondary exit [misrac-rule-15p4]
    do {
        switch (n) {
        case 0:
            goto out;
        case 1:
            goto out;
        default:
            break;
        }
    } while (1);

out:
    ;
}

void f2(int n) {
    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: loop contains more than one secondary exit [misrac-rule-15p4]
    while (1) {
        if (!n)
            break;

        while (n--) {
            if (n < 0)
                goto out;
        }
    }

out:
    ;
}

void f3(int m, int n, int o) {
    for (int i = 0; i < m; ++i) {
        for (int j =  0; j < n; ++j) {
            // CHECK-MESSAGES: [[@LINE+1]]:13: warning: loop contains more than one secondary exit [misrac-rule-15p4]
            for (int k = 0; k < o; ++k) {
                switch (n) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                default:
                    goto out;
                }

                if (i * j * k != 0)
                    break;
            }
        }
    }
out:
    ;
}

void f4(void) {
    int data[] = { 0, 1, 2, 3, 4 };

    // CHECK-MESSAGES: [[@LINE+1]]:5: warning: loop contains more than one secondary exit [misrac-rule-15p4]
    for (auto n : data) {
        if (n == 0)
            break;

        if (n == 1)
            break;
    }
}
