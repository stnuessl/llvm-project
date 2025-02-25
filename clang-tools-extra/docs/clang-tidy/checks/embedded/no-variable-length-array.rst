.. title:: clang-tidy - embedded-no-variable-length-array

embedded-no-variable-length-array
=================================

Detects declarations of variable-length arrays (VLAs).
This check is provided for convenience in embedded software development.
Consider using the easily available
`-Wvla <../../../../../../../docs/html/DiagnosticsReference.html#wvla>`_
diagnostic flag in `clang <https://clang.llvm.org/>`_ or
`gcc <https://gcc.gnu.org/>`_ as an alternative, as it may be a more effective
solution.


Examples:

.. code-block:: C++

  void f(int n) {
      // Not ok
      int a[n];
  }


Options
-------

This check does not support any options.
