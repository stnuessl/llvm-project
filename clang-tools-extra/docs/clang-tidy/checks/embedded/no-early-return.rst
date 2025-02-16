.. title:: clang-tidy - embedded-no-early-return

embedded-no-early-return
========================

Detects early return statements that are not placed at the end of
a function body’s compound statement.

Examples:

.. code-block:: c++

  void f(int n) {
      if (n < 0)
          /* Not ok */
          return;

      /* ... */
  }


Options
-------

This check does not support any options.

