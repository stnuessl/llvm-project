.. title:: clang-tidy - misc-no-variable-array

misc-no-variable-array
======================

Detects variable length array declarations, which are considered bad practice
according to coding standards like `MISRA C <https://misra.org.uk>`_.

Examples:

.. code-block:: c++

  void fn(int n) {
    // Not ok
    int a[n];
  }


Options
-------

This check does not support any options.
