.. title:: clang-tidy - misrac-rule-8p10

misrac-rule-8p10
================

Detects issues in accordance with MISRA C rule 8.10:
"`An inline function shall be declared with the static storage class`".

Examples:

.. code-block:: c++

  // Not ok
  inline void f() {}


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

