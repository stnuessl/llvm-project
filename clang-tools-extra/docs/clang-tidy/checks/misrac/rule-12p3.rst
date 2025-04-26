.. title:: clang-tidy - misrac-rule-12p3

misrac-rule-12p3
================

Detects issues in accordance with MISRA C rule 12.3:
"`The comma operator should not be used`".

Examples:

.. code-block:: c++

   // Ok
   int a, b;

   // Not ok
   a = 0, b = 0;


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

.. option:: ReportUseInForLoops

  Additionally, emit a diagnostic about the use of the comma operator in
  ``for`` loops:

  .. code-block:: c++

    for (int i = 0, j = 0; /* ... */; ++i, ++j) {
        /* ... */
    }

  Defaults to ``false``.


