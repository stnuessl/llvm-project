.. title:: clang-tidy - misrac-rule-18p5

misrac-rule-18p5
================

Detects issues in accordance with MISRA C rule 18.5:
"`Declarations should contain no more than two levels of pointer nesting`".

Examples:

.. code-block:: C++

  // Ok
  int **p1;

  // Not Ok
  int ***p2;


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

.. option:: MaxNestingLevel

  Specifies the maximum allowed pointer nesting level.
  Default is ``2``.

