.. title:: clang-tidy - misrac-rule-10p7

misrac-rule-10p7
================

Detects issues in accordance with MISRA C rule 10.7:
"`If a composite expression is used as one operand of an operator in
which the usual arithmetic conversions are performed then the other
operand shall not have wider essential type`".

Examples:

.. code-block:: C++

  void g(uint32_t u32);

  void f(uint32_t u32, uint16_t u16) {
    // Not ok
    g(2u * u32 + 4u * u16);
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


