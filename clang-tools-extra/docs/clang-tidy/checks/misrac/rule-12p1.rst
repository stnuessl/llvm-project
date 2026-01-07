.. title:: clang-tidy - misrac-rule-12p1

misrac-rule-12p1
================

Detects issues in accordance with MISRA C rule 12.1:
"`The precedence of operators within expressions should be made explicit`".


Examples:

.. code-block:: c++

  // Not ok
  sizeof x + y

  // Not ok
  a = b << c + 1;


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.


Alternatives
------------

.. note::

  Consider using the easily available `-Wparentheses
  <../../../../../../../docs/html/DiagnosticsReference.html#wparentheses>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_.


