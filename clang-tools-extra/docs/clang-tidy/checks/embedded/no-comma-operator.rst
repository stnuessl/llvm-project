.. title:: clang-tidy - embedded-no-comma-operator

embedded-no-comma-operator
==========================

Detects the use of the comma operator which is considered to be
bad practice by coding standards like `MISRA C <https://misra.org.uk>`_.

Examples:

.. code-block:: c++

   // Ok
   int a, b;

   // Not ok
   a = 0, b = 0;


Options
-------

.. option:: StrictMode

  Additionally, emit a diagnostic about the use of the comma operator in
  ``for`` loops:

  .. code-block:: c++

    for (int i = 0, j = 0; /* ... */; ++i, ++j) {
        /* ... */
    }

  Defaults to `false`.


.. option:: ReportAtMacroExpansion

  If the comma operator is used within a preprocessor macro, do not generate
  a diagnostic at the macro definition location. Instead, generate the
  diagnostic at the location where the macro is expanded.

  .. code-block:: c++

     #define MACRO(x) (f(x), 0)

     // Emit diagnostic on next line
     MACRO(false);

  Defaults to `false`.

