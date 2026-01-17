.. title:: clang-tidy - misrac-rule-15p4

misrac-rule-15p4
================

Detects issues in accordance with MISRA C rule 15.4:
"`There should be no more than one break or goto statement used to terminate
any iteration statement`".

Examples:

.. code-block:: c++

   void f(int m, int n) {
     // Not ok
     while (1) {
         if (m < 0) break;
         if (n > 0) break;
         /* ... */
     }
   }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.


