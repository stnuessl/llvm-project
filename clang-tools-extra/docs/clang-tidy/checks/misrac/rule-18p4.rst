.. title:: clang-tidy - misrac-rule-18p4

misrac-rule-18p4
================

Detects issues in accordance with MISRA C rule 18.4:
"`The +, -, += and -= operators should not be applied to an expression of
pointer type`".

Examples:

.. code-block:: c++

  // ok
  p[i] = 0;

  // Not ok
  *(p + i) = 0;

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

