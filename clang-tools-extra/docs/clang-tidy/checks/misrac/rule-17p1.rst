.. title:: clang-tidy - misrac-rule-17p1

misrac-rule-17p1
================

Detects issues in accordance with MISRA C rule 17.1:
"`The features of <stdarg.h> shall not be used`".

Examples:

.. code-block:: c++


  void f(int n, ...) {
    // Not ok
    va_list vargs;

    // Not ok
    va_start(n, vargs);

    // ...
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

