.. title:: clang-tidy - embedded-no-flexible-array-member

embedded-no-flexible-array-member
=================================

Detects flexible array member declarations.


Examples:

.. code-block:: C++

  struct x {
    int size;
    // Not ok
    char data[];
  };


Options
-------

This check does not support any options.
