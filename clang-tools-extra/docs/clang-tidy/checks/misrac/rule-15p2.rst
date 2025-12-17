.. title:: clang-tidy - misrac-rule-15p2

misrac-rule-15p2
================

Detects issues in accordance with MISRA C rule 15.2:
"`The goto statement shall jump to a label declared later in the same
function`".

Examples:

.. code-block:: c++

  // Not ok
  l1: if (--n) goto l1;

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

