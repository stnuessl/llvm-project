.. title:: clang-tidy - misrac-rule-19p2

misrac-rule-19p2
================

Detects issues in accordance with MISRA C rule 19.2:
"`The union keyword should not be used`".

Examples:

.. code-block:: c++

  // Not ok
  union a { /* ... */ };


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

