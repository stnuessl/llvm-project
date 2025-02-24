.. title:: clang-tidy - embedded-default-case-position

embedded-default-case-position
==============================

Detects default statements within switch statements that are not located at
the first or last position in the case list.

Examples:

.. code-block:: C++

  switch (value) {
  case 0: break;
  // Not ok
  default: break;
  case 1: break;
  }


Options
-------

This check does not support any options.
