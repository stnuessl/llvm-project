// RUN: %check_clang_tidy %s misrac-rule-3p2 %t

// Lorem ipsum dolor sit amet

/*
 * Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex \
 * sapien vitae pellentesque sem placerat.
 */

// CHECK-MESSAGES: [[@LINE+1]]:80: warning: disallowed '\' character in // comment [misrac-rule-3p2]
// Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex \
    sapien vitae pellentesque sem placerat.


// CHECK-MESSAGES: [[@LINE+6]]:80: warning: disallowed '\' character in // comment [misrac-rule-3p2]
// CHECK-MESSAGES: [[@LINE+6]]:76: warning: disallowed '\' character in // comment [misrac-rule-3p2]
// CHECK-MESSAGES: [[@LINE+6]]:73: warning: disallowed '\' character in // comment [misrac-rule-3p2]
// CHECK-MESSAGES: [[@LINE+6]]:76: warning: disallowed '\' character in // comment [misrac-rule-3p2]
// CHECK-MESSAGES: [[@LINE+6]]:75: warning: disallowed '\' character in // comment [misrac-rule-3p2]
// CHECK-MESSAGES: [[@LINE+6]]:76: warning: disallowed '\' character in // comment [misrac-rule-3p2]
// Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex \
    sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus \
    duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar \
    vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl \
    malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class \
    aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos \
    himenaeos.


