.. title:: clang-tidy - misrac-rule-16p4

misrac-rule-16p4
================

Detects issues in accordance with MISRA C rule 16.4:
"`Every switch statement shall have a default label`".


Examples:

.. code-block:: c++

  // Not ok
  switch (n) {
  case 0: break;
  case 1: break;
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
  `-Wswitch-default
  <../../../../../../../docs/html/DiagnosticsReference.html#wswitch-default>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_ or
  `gcc <https://gcc.gnu.org/>`_ as an alternative, as it may be a more effective
  solution.


