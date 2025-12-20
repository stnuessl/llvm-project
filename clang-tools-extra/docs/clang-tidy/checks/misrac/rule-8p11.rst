.. title:: clang-tidy - misrac-rule-8p11

misrac-rule-8p11
================

Detects issues in accordance with MISRA C rule 8.11:
"`When an array with external linkage is declared, its size should be
explicitly specified`".

Examples:

.. code-block:: c++

  // Not ok
  extern int a[];


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

