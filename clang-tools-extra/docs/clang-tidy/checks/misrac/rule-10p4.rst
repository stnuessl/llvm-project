.. title:: clang-tidy - misrac-rule-10p4

misrac-rule-10p4
================

Detects issues in accordance with MISRA C rule 10.4:
"`Both operands of an operator in which the usual arithmetic conversions
are performed shall have the same essential type category`".

Examples:

.. code-block:: c++

  void f(int i) {
    // Not ok - float and signed
    float f = -1.0f * i;
  }


.. Note::

  The following operators are subject to this rule:
    * All arithmetic operators: ``+``, ``-``, ``*``, ``/``, ``%`` and
      respective compound assignments operators
    * All bitwise operators: ``|``, ``&``, ``^`` and
      respective compound assignments operators
    * All comparison operators: ``==``, ``!=``, ``<``, ``>``, ``<=``, ``>=``
    * The conditional operator: ``?``


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: IgnoreAssignments

  Boolean value. Set to ``true`` to ignore all compound assignments.
  Default is ``false``.


Shared Options
--------------

.. option:: misrac.shared-options.TreatAsBoolean:

  Semicolon-separated list of type names which will be treated
  as ``boolean``. Empty by default.

