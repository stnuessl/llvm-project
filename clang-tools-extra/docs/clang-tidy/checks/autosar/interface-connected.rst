.. title:: clang-tidy - autosar-interface-connected

autosar-interface-connected
===========================

.. contents::

Overview
--------

This check analyzes automatically generated AUTOSAR interfaces and emits
appropriate diagnostics if unconnected interfaces are detected.

Example
-------

Given the line of code

.. code-block:: c

    (void) Rte_Write_PP_PortA_DataElement1(Value);


this check will emit the diagnostic messages

.. code-block:: sh

    Swc.c:80:12: warning: usage of unconnected interface "Rte_Write_PP_PortA_DataElement1" [autosar-interface-connected]
    80 |     (void) Rte_Write_PP_PortA_DataElement1(Value);
    81 |            ^
    Rte_Swc.h:7:45: note: expanded from macro 'Rte_Write_PP_PortA_DataElement1'
     7 | #define Rte_Write_PP_PortA_DataElement1(Value) ((void) Value, RTE_E_UNCONNECTED)
     8 |                                                ^


to highlight that an unconnected interface is used.


Options
-------

This section briefly describes all available options for
`autosar-interface-connected <interface-connected.html>`_.

.. option:: MacroRegex

    A regular expression used to filter preprocessor macros considered by this
    check. The default value is ``Rte_.+``.

.. option:: FunctionRegex

    A regular expression used to filter function definitions considered by this
    check. The default value is ``Rte_.+``.


Usage with clangd
-----------------

No issues or limitations are known meaning that this check should *just work*
if used in combination with `clangd <https://clangd.llvm.org/>`_.


Rationale
---------

Usually, AUTOSAR-related tooling allows for developing software components in
isolation, meaning that these components are not yet integrated into the ECU
software. This is achieved by stubbing the interfaces used by a software
component and making them return the value **RTE_E_UNCONNECTED**. Additionally,
in some software configurations, it might not even be desirable to have every
interface of a software component connected. However, this often leads to
delivered software components that are not fully or correctly integrated due to
unconnected interfaces. Sometimes, interfaces are also disconnected by accident
due to the complex nature of merging AUTOSAR XML files. This check supports the
development of ECU software within an AUTOSAR context by warning about
such unconnected interfaces.


Limitations
-----------

For technical reasons related to how AUTOSAR software is generated, this check
will not detect all unconnected interfaces. BSW service ports, in particular,
seem problematic. However, due to the check's fast feedback cycle - especially
in combination with `clangd <https://clangd.llvm.org/>`_ and its ease of setup,
it remains a useful tool for ECU software developers.


