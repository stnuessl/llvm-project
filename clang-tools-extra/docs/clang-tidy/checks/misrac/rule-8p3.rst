.. title:: clang-tidy - misrac-rule-8p3

misrac-rule-8p3
===============

Detects issues in accordance with MISRA C rule 8.3:
"`All declarations of an object or function shall use the same names and type
qualifiers`".

Examples:

.. code-block:: c++

  void f(int *const p);
  // Not ok
  void f(int *p) {}


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

