.. title:: clang-tidy - misrac-rule-18p8

misrac-rule-18p8
================

Detects issues in accordance with MISRA C rule 18.8:
"`Variable-length array types shall not be used`".


Examples:

.. code-block:: C++

  void f(int n) {
      // Not ok
      int a[n];
  }

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.


Alternatives
------------

.. note::

  Consider using the easily available
  `-Wvla <../../../../../../../docs/html/DiagnosticsReference.html#wvla>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_ or
  `gcc <https://gcc.gnu.org/>`_ as an alternative, as it may be a more effective
  solution.


