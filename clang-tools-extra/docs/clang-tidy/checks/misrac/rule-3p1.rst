.. title:: clang-tidy - misrac-rule-3p1

misrac-rule-3p1
===============

Detects issues in accordance with MISRA C rule 3.1:
"`The character sequences /* an d // shall not be used within a comment`".

Examples:

.. code-block:: c++

  void f() {
    // Not ok
    /* Begin section
        g();
    /* End section */
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
  `-Wcomment <../../../../../../../docs/html/DiagnosticsReference.html#wcomment>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_ or
  `gcc <https://gcc.gnu.org/>`_ as an alternative.

