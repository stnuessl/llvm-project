.. title:: clang-tidy - misrac-rule-13p4

misrac-rule-13p4
================

Detects issues in accordance with MISRA C rule 13.4:
"`The result of an assignment operator should not be used`".

Examples:

.. code-block:: c++

  // Not ok
  if (a = f()) { /* ... */ }


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
  `-Wparentheses
  <../../../../../../../docs/html/DiagnosticsReference.html#wparentheses>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_.


