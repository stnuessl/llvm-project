.. title:: clang-tidy - embedded-missing-else

embedded-missing-else
=====================

Detects `if` ... `else if` constructs that lack a terminating `else`
statement. This pattern may be interpreted similarly to a `switch`
statement without a `default` case.

Examples:

.. code-block:: c++

  // Not ok
  if (Value == ValueA) { /* ... */ }
  else if (Value == ValueB) { /* ... */ }

  // Ok
  if (Value == ValueA) { /* ... */ }
  else if (Value == ValueB) { /* ... */ }
  else { /* ... */ }


Options
-------

.. Option:: IndentationStyle

  Applies the specified indentation style within proposed fixes.
  Possible values: ``None``, ``Allman`` and ``K&R``. Default is ``Allman``.

.. Option:: Indentation

  The number of spaces used within proposed fixes per indentation level.

