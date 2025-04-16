.. title:: clang-tidy - misra-rule-10p1

misra-rule-10p1
===============

Detects issues in accordance with MISRA C rule 10.1:
"`Operands shall not be of an inappropriate essential type`".

Examples:

.. code-block:: c++

  void f(bool b) {
    // Not ok - boolean used in relational operation
    if (b > 0) { /* ... */ }
  }

  void g(enum e e) {
    // Not ok - enum used in arithmetic operation
    e = e - 1;
  }

  void h(int i) {
    // Not ok - boolean used in arithmetic operation
    i *= (i != 0);
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

