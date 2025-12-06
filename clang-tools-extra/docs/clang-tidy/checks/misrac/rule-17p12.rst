.. title:: clang-tidy - misrac-rule-17p12

misrac-rule-17p12
=================

Detects issues in accordance with MISRA C rule 17.12:
"`A function identifier should only be used with either a preceding &, or
with a parenthesized parameter list`".

Examples:

.. code-block:: c++


  void f();

  void g() {
    // Not ok
    void (*fn)() = f;
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

