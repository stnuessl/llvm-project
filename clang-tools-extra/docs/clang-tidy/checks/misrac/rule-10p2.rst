.. title:: clang-tidy - misrac-rule-10p2

misrac-rule-10p2
================

Detects issues in accordance with MISRA C rule 10.2:
"`Expressions of essentially  character type shall not be used inappropriately
in addition and subtraction operations`".

Examples:

.. code-block:: c++

  enum e { E0 };

  void f(char c1, char c2) {
    // Not ok
    char c3 = c1 + c2;

    // Not ok
    c3 -= 1.0f;

    // Not ok
    c3 = c3 + E0;
  }


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.


Shared Options
--------------

.. option:: misrac.shared-options.TreatAsBoolean:

  Semicolon-separated list of type names which will be treated
  as ``boolean``. Empty by default.

