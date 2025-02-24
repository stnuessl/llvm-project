.. title:: clang-tidy - embedded-missing-static

embedded-missing-static
=======================

Detects inline function declarations that lack the static storage qualifier.

Examples:

.. code-block:: c++

  // Not ok
  inline void f() {}


Options
-------

This check does not support any options.

