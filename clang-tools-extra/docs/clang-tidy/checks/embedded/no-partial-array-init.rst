.. title:: clang-tidy - embedded-no-partial-array-init

embedded-no-partial-array-init
==============================

Detects partial array initializiations.

Examples:

.. code-block:: C

  // Ok (Idiomatic Zero Initializer)
  int a[2] = {0};

  // Not ok
  int a[2] = {1};


Options
-------

This check does not support any options.
