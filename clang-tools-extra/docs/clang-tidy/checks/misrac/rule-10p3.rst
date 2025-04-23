.. title:: clang-tidy - misrac-rule-10p3

misrac-rule-10p3
================

Detects issues in accordance with MISRA C rule 10.3:
"`The value of an expression shall not be assigned to an object with a
narrower essential type or of a different essential type category`".

Examples:

.. code-block:: c++

  int g(int i);

  bool f(void) {
    // Not ok - narrower type
    unsigned char c = 256u;
    unsigned int u;

    // Not ok - unsigned and signed
    u = g(0);

    // Not ok - signed and character
    (void) g('\0');

    // Not ok - boolean and signed
    return 0;
  }


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.


Shared Options
--------------

.. option:: misrac.shared-options.TreatAsBoolean:

  Semicolon-separated list of type names which will be treated
  as ``boolean``. Empty by default.

