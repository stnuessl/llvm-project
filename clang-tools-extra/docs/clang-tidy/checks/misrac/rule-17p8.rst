.. title:: clang-tidy - misrac-rule-17p8

misrac-rule-17p8
================

Detects issues in accordance with MISRA C rule 17.8:
"`A function parameter should not be modiﬁed`".


Examples:

.. code-block:: c++

  void f(int a) {
      int *p;

      // Not ok
      a = 0;

      // Not ok
      p = &a;
  }


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: AddrOpDiagLevel

  Set the diagnostic level for using the address operator (``&``) on a
  parameter.

  Value must be one of  ``Warning``, ``Remark``, or ``Ignored``.
  Defaults to ``Warning``.

