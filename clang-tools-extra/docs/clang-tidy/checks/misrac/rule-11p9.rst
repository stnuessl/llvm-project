.. title:: clang-tidy - misrac-rule-11p9

misrac-rule-11p9
================

Detects issues in accordance with MISRA C rule 11.9:
"`The macro NULL shall be the only permitted form of integer null pointer
constant`".

Examples:

.. code-block:: c++

  // Not ok
  int *ptr = 0;

Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

.. option:: AllowedNullPointerConstants

  A semicolon-separated list of supported null pointer constants. Its primary
  purpose is to provide appropriate code-fix suggestions. Note that, due to the
  implementation-defined nature of ``NULL``, it may not be possible to disallow
  its use.
  The default value is ``NULL``.

