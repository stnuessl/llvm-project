.. title:: clang-tidy - misrac-rule-17p7

misrac-rule-17p7
================

Detects issues in accordance with MISRA C rule 17.7:
"`The value returned by a function having non-void return type shall be used`".

Examples:

.. code-block:: c++

  int g();

  void f() {
    // Not ok
    g();
  }


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

