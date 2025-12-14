.. title:: clang-tidy - misrac-rule-16p2

misrac-rule-16p2
================

Detects issues in accordance with MISRA C rule 16.2:
"`A switch label shall only be used when the most closely-enclosing compound
statement is the body of a switch statement`".


Examples:

.. code-block:: C++

  switch (value) {
  case 0:
    if (/* ... */) {
  // Not ok
  case 1: break;
    }
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

