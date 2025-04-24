.. title:: clang-tidy - misrac-rule-10p6

misrac-rule-10p6
================

Detects issues in accordance with MISRA C rule 10.6:
"`The value of a composite expression  shall not be assigned to an object
with wider essential type`".

Examples:

.. code-block:: C++

  void f(uint16_t u16) {
    // Not ok
    uint32_t u32 = 2u * u16;
  }

  int32_t g(int16_t s16) {
    // Not ok
    return s16 + s16;
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

