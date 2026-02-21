.. title:: clang-tidy - misrac-rule-6p3

misrac-rule-6p3
===============

Detects issues in accordance with MISRA C rule 6.3:
"`A bit field shall not be declared as a member of a union`".

Examples:

.. code-block:: C++

  // Not ok
  union u {
      unsigned int n : 1;
  };


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.


