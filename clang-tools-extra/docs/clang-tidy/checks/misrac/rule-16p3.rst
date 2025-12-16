.. title:: clang-tidy - misrac-rule-16p3

misrac-rule-16p3
================

Detects issues in accordance with MISRA C rule 16.3:
"`An unconditional break statement shall terminate every switch-clause`".

Examples:

.. code-block:: C++

  switch (value) {
  // Not ok
  case 0: f();
  case 1: break;
  default: break;
  }


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

