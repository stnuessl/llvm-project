.. title:: clang-tidy - misrac-rule-15p6

misrac-rule-15p6
================

Detects issues in accordance with MISRA C rule 15.6:
"`The body of an iteration-statement or a selection-statement shall be a
compound-statement`".


Examples:

.. code-block:: c++

  // Not ok
  while (1) f();

  // Ok
  while (1) { f(); }


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

