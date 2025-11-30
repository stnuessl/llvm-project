.. title:: clang-tidy - misrac-rule-9p3

misrac-rule-9p3
===============

Detects issues in accordance with MISRA C rule 9.3:
"`Arrays shall not be partially initialized`".


Examples:

.. code-block:: C

  // Ok (Idiomatic Zero Initializer)
  int a[2] = {0};

  // Not ok
  int a[2] = {1};


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

