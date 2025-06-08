.. title:: clang-tidy - misrac-rule-15p7

misrac-rule-15p7
================

Detects issues in accordance with MISRA C rule 15.7:
"`All if ... else if constructs shall be terminated with an else statement`".

Examples:

.. code-block:: c++

  void f(int a, int b) {
      if (a == 0) { /* ... */ }
      else if (b == 0) { /* ... */ }
      // Not Ok
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

