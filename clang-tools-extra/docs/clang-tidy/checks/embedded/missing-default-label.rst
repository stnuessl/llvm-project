.. title:: clang-tidy - embedded-missing-default-label

embedded-missing-default-label
==============================

Detects `switch` statements without a `default` label.
This check is provided for convenience in embedded software development.
Consider using the easily available
`-Wswitch-default <../../../../../../../docs/html/DiagnosticsReference.html#wswitch-default>`_
diagnostic flag in `clang <https://clang.llvm.org/>`_ or
`gcc <https://gcc.gnu.org/>`_ as an alternative, as it may be a more effective
solution.

Examples:

.. code-block:: c++

  // Not ok
  switch (value) {
  case 0: break;
  case 1: break;
  }


Options
-------

This check does not support any options.

