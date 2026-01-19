.. title:: clang-tidy - misrac-rule-8p12

misrac-rule-8p12
================

Detects issues in accordance with MISRA C rule 8.12:
"`Within an enumerator list, the value of an implicitly-specified enumeration
constant shall be unique`".

Examples:

.. code-block:: c++

  // Not ok
  enum e1 { e11, e12 = 0 };


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.
