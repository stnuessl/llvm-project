.. title:: clang-tidy - misrac-rule-16p5

misrac-rule-16p5
================

Detects issues in accordance with MISRA C rule 16.5:
"`A default label shall appear as either the ﬁrst or the last switch label
of a switch statement`".


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

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

