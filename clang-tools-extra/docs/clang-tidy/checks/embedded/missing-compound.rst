.. title:: clang-tidy - embedded-missing-compound

embedded-missing-compound
=========================

Detects `do`, `for`, `while`, `if`, `else`, and `switch` statements where the
body is not a compound statement.

Examples:

.. code-block:: c++

  // Not ok
  while (1) f();

  // Ok
  while (1) { f(); }


Options
-------

This check does not support any options.
