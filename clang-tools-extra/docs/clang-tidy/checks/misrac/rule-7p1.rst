.. title:: clang-tidy - misrac-rule-7p1

misrac-rule-7p1
===============

Detects issues in accordance with MISRA C rule 7.1:
"`Octal constants shall not be used`".

Examples:

.. code-block:: C++

  // Not ok
  int num = 0777;


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

