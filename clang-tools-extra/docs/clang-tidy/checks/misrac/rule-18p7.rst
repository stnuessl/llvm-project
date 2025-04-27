.. title:: clang-tidy - misrac-rule-18p7

misrac-rule-18p7
================

Detects issues in accordance with MISRA C rule 18.7:
"`Flexible array members shall not be declared`".

Examples:

.. code-block:: C++

  struct x {
    int size;
    // Not ok
    char data[];
  };


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

