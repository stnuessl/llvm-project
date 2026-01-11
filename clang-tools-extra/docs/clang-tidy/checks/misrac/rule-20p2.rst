.. title:: clang-tidy - misrac-rule-20p2

misrac-rule-20p2
================

Detects issues in accordance with MISRA C rule 20.2:
"`The ', " or \ characters and the /* or // character sequences shall not occur
in a header file name`".

Examples:

.. code-block:: c++

  // Not ok
  #include "file'h"


Options
-------

.. option:: ReportAtMacroDefinition

  Instead of emitting diagnostic messages at the location of macro expansions,
  emit them at the location of the macro definition. Defaults to ``false``.

.. option:: ExtendToCPlusPlus

  Also enable this check for C++ source files.
  Defaults to ``true``.

.. option:: Regex

  Define a regular expression which will be used to detect additional invalid
  patterns in header file names.
  Defaults to ``""`` (empty string).

