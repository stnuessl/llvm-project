.. title:: clang-tidy - misrac-rule-14p4

misrac-rule-14p4
================

Detects issues in accordance with MISRA C rule 14.4:
"`The controlling expression of an if statement and the controlling
expression of an iteration- statement shall have essentially Boolean type`".

Examples:

.. code-block:: C++

  int g(void);

  void f(int *p) {
    int value;

    // Not ok - pointer
    while (p) {}

    // Not ok - signed
    if (*p) {}

    // Not ok - signed
    if (value = g()) {}
  }


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: StrictWhile

  Boolean value. Additionally emit diagnostics for ``while (1)`` loops.
  Default value is ``false``.

.. option:: StrictDo

  Boolean value. Additionally emit diagnostics for
  ``do { /* ... */ } while (0)`` loops. Default value is ``false``.


Shared Options
--------------

.. option:: misrac.shared-options.TreatAsBoolean:

  Semicolon-separated list of type names which will be treated
  as ``boolean``. Empty by default.
