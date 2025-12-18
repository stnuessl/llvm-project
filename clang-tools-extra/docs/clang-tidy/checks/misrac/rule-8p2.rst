.. title:: clang-tidy - misrac-rule-8p2

misrac-rule-8p2
===============

Detects issues in accordance with MISRA C rule 8.2:
"`Function types shall be in prototype form with named parameters`".

Examples:

.. code-block:: c++

  // Not ok (missing prototype)
  void f();

  // Not ok (missing parameter name)
  void g(int)


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

