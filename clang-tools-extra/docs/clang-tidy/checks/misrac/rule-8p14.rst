.. title:: clang-tidy - misrac-rule-8p14

misrac-rule-8p14
================

Detects issues in accordance with MISRA C rule 8.14:
"`The restrict type qualifier shall not be used`".

Examples:

.. code-block:: c++

  // Not ok
  void f(int *restrict p);


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

