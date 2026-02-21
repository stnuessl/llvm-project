.. title:: clang-tidy - misrac-rule-11p2

misrac-rule-11p2
================

Detects issues in accordance with MISRA C rule 11.2:
"`Conversions shall not be performed between a pointer to an incomplete type
and any other type`".

Examples:

.. code-block:: c++

  struct s;

  void f(struct s *s) {
    int n;

    // Ok
    s = (struct s *) 0;

    // Not ok
    s = (struct s *) &n;
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

