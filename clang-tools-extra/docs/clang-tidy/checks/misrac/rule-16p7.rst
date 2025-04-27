.. title:: clang-tidy - misrac-rule-16p7

misrac-rule-16p7
================

Detects issues in accordance with MISRA C rule 16.7:
"`A switch-expression shall not have essentially Boolean type`".


Examples:

.. code-block:: C++

  void f(int n) {
    // Not ok
    switch (n != 0) { /* ... */ }
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


Alternatives
------------

.. note::

  Consider using the easily available
  `-Wswitch-bool
  <../../../../../../../docs/html/DiagnosticsReference.html#wswitch-bool>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_ or
  `gcc <https://gcc.gnu.org/>`_ as an alternative, as it may be a more effective
  solution.


