.. title:: clang-tidy - misrac-rule-8p8

misrac-rule-8p8
===============

Detects issues in accordance with MISRA C rule 8.8:
"`The static storage class specifier shall be used in all declarations of
objects and functions that have internal linkage`".


Examples:

.. code-block:: c++

  // Not ok
  static void f(void);
  void f(void) {}


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

