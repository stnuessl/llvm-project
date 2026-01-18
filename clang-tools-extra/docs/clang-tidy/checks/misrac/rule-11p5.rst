.. title:: clang-tidy - misrac-rule-11p5

misrac-rule-11p5
================

Detects issues in accordance with MISRA C rule 11.5:
"`A conversion should not be performed from pointer to void into pointer to
object`".

Examples:

.. code-block:: c++

  void f(void *p) {
    // Not ok
    int *q = (int *) p;
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.


