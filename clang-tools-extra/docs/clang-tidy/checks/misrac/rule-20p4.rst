.. title:: clang-tidy - misrac-rule-20p4

misrac-rule-20p4
================

Detects issues in accordance with MISRA C rule 20.4:
"`A macro shall not be defined with the same name as a keyword`".

Examples:

.. code-block:: c++

  // Not ok
  #define const


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

.. option:: Allowed

  Semicolon-separated list of keywords for which no diagnostic is emitted when
  a macro with the same name as a keyword is defined.
  Defaults to ``""`` (empty string).


Alternatives
------------

.. note::

  Consider using the easily available
  `-Wkeyword-macro <../../../../../../../docs/html/DiagnosticsReference.html#wkeyword-macro>`_
  diagnostic flag in `clang <https://clang.llvm.org/>`_
  as it may be a more effective solution.

