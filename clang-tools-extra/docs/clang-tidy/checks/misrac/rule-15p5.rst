.. title:: clang-tidy - misrac-rule-15p5

misrac-rule-15p5
================

Detects issues in accordance with MISRA C rule 15.5:
"`A function should have a single point of exit at the end`".

Examples:

.. code-block:: c++

  void f(void *p) {
      if (!p) {
        // Not ok
        return;
      }

      /* ... */
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

