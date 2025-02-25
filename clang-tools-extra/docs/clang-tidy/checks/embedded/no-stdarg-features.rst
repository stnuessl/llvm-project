.. title:: clang-tidy - embedded-no-stdarg-features

embedded-no-stdarg-features
===========================

Detects usage of types and macros from `stdarg.h`.

Examples:

.. code-block:: c++

  // Not ok
  va_list vargs;

  // Not ok
  va_start(vargs, /* ... */);

  /* ... */


Options
-------

This check does not support any options.

