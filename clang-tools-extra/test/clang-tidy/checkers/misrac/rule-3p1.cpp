// RUN: %check_clang_tidy %s misrac-rule-3p1 %t

// Lorem ipsum dolor sit amet //

// CHECK-MESSAGES: [[@LINE+1]]:31: warning: disallowed character sequence '{{/}}*' in comment [misrac-rule-3p1]
// Lorem ipsum dolor sit amet /* Lorem ipsum dolor sit amet */

// CHECK-MESSAGES: [[@LINE+1]]:31: warning: disallowed character sequence '{{/}}/' in comment [misrac-rule-3p1]
/* Lorem ipsum dolor sit amet // consectetur adipiscing elit. Quisque */

// CHECK-MESSAGES: [[@LINE+4]]:30: warning: disallowed character sequence '{{/}}/' in comment [misrac-rule-3p1]
// CHECK-MESSAGES: [[@LINE+6]]:34: warning: disallowed character sequence '{{/}}*' in comment [misrac-rule-3p1]
/*
 * Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex
 * sapien vitae pellentesque // sem placerat. In id cursus mi pretium tellus
 * duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus
 * fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada
 * lacinia integer nunc posuere. /* Ut hendrerit semper vel class aptent taciti
 * sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.
 */


