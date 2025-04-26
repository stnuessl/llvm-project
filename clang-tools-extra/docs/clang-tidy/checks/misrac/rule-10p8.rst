.. title:: clang-tidy - misrac-rule-10p8

misrac-rule-10p8
================

Detects issues in accordance with MISRA C rule 10.8:
"`The value of a composite expression shall not be cast to a different
essential type category or a wider essential type`".

Examples:

.. code-block:: c++

  void g(double f64);

  void f(float f32a, float f32b) {
    // Not ok - cast to different type category
    uint32_t u32 = (uint32_t) (f32a + f32b);

    // Not ok - cast to wider type
    g((double) (f32a * f32b));
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
