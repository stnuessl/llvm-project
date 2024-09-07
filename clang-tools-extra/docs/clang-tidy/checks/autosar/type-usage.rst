.. title:: clang-tidy - autosar-type-usage

autosar-type-usage
==================

.. contents::

Overview
--------

This check detects incorrect usage of AUTOSAR data types ([1]_, [2]_, etc.)
and their associated values.

.. code-block:: c

   boolean Value = 0u;

   Std_ReturnType RetVal = 0u;


FIXME: Describe what patterns does the check detect and why. Give examples.

Options
-------

This section briefly describes all available options for
`autosar-type-usage <type-usage.html>`_.

.. option:: Option.Add

    A **semicolon-separated list** of type names. This option specififes
    additional types that will get analyzed by this check.
    Each entry in the list requires an additional option, see  <Added-Type>.

.. option:: Option.Remove

    A **semicolon-separated list** of type names. This option excludes
    the specified types from getting analyzed by this check.

.. option:: Option.Booleans


.. option:: <Added-Type>

    A **semicolon-separated list** of macro names. This option is used
    to specify all the potential values that <Added-Type> can assume.


How-To
------

Don't allow logical not on 'boolean':

CheckOptions:
    autosar-type-usage.BoolTypes: ''


Usage with clangd
-----------------

No issues or limitations are known meaning that this check should *just work*
if used in combination with `clangd <https://clangd.llvm.org/>`_.


Rationale
---------

Easy to make mistakes by mixing up macros. People like to shortcut
type related issues

Relation to MISRA C
-------------------

FIXME:
See  [3]_

* Essential type system.
* MISRA is unaware of AUTOSAR types
* Preprocessor macros do not contain associated type information.

References
----------

Please note that this list is not extensive and the references also might be
out of date. In any case, the links should provide sufficient information
to start research on the AUTOSAR type system.


.. [1]
    `Specification of Standard Types - AUTOSAR CP R21-11
    <https://www.autosar.org/fileadmin/standards/R21-11/CP/AUTOSAR_SWS_StandardTypes.pdf>`_

.. [2]
    `Specification of ECU State Manager AUTOSAR CP R21-11
    https://www.autosar.org/fileadmin/standards/R21-11/CP/AUTOSAR_SWS_ECUStateManager.pdf>`

.. [3]
   `MISRA Homepage
   <https://misra.org.uk>`
