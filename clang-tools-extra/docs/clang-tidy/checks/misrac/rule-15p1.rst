.. title:: clang-tidy - misrac-rule-15p1

misrac-rule-15p1
================

Detects issues in accordance with MISRA C rule 15.1:
"`The goto statement should not be used`".

Examples:

.. code-block:: C++

   // Not ok
   goto out;


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

