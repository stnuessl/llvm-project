.. title:: clang-tidy - misrac-rule-8p4

misrac-rule-8p4
===============

Detects issues in accordance with MISRA C rule 8.4:
"`A compatible declaration shall be visible when an object or function with
external linkage is defined`".

Examples:

.. code-block:: c++

  // Ok
  void f(void);
  void f(void) {}

  // Not ok
  void g(void) {}


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

.. option:: IgnoreFunctions

  A semicolon-separated list of regular expressions identifying functions that
  will be ignored by this check. This is useful for avoiding warnings for
  program entry points.
  The default value is ``^::main$``.
