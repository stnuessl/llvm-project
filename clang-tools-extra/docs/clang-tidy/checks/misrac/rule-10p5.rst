.. title:: clang-tidy - misrac-rule-10p5

misrac-rule-10p5
================

Detects issues in accordance with MISRA C rule 10.5:
"`The value of an expression should not be cast to an inappropriate
essential type`".

Examples:

.. code-block:: c++

  void f(int i) {
    // Not ok - signed to boolean
    bool b = (bool) i;
  }

  void g(double d) {
    // Not ok - floating to character
    char c = (char) d;
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


