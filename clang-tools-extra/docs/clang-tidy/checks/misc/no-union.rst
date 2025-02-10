.. title:: clang-tidy - misc-no-union

misc-no-union
=============

Detects the declarations of unions which is considered to be
bad practice by coding standards like `MISRA C <https://misra.org.uk>`_.

Examples:

.. code-block:: c++

    // Not ok
    union a { /* ... */ };

Options
-------

This check does not support any options.
