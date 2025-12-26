.. title:: clang-tidy - misrac-rule-7p3

misrac-rule-7p3
===============

Detects issues in accordance with MISRA C rule 7.3:
"`The lowercase character 'l' shall not be used in a literal suffix`".

Examples:

.. code-block:: C++

  // Not ok
  long num = 0l;


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

