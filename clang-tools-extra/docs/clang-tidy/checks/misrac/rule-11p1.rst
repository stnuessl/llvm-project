.. title:: clang-tidy - misrac-rule-11p1

misrac-rule-11p1
================

Detects issues in accordance with MISRA C rule 11.1:
"`Conversions shall not be performed between a pointer to a function
and any other type`".

Examples:

.. code-block:: c++

  void f(void *p) {
    void (*fp)(void);

    // Ok
    fp = NULL;

    // Not ok
    fp = (void (*)(void) p;
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.


