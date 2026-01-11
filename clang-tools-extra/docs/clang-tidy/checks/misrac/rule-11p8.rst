.. title:: clang-tidy - misrac-rule-11p8

misrac-rule-11p8
================

Detects issues in accordance with MISRA C rule 11.8:
"`A cast shall not remove any const or volatile qualification from the type
pointed to by a pointer"`.


Examples:

.. code-block:: c++

  void f(const int *p) {
    // Not ok
    int *q = (int *) p;
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

  Consider using the easily available `-Wcast-qual
  <../../../../../../../docs/html/DiagnosticsReference.html#wcast-qual>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_ or
  `gcc <https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html>`_.


