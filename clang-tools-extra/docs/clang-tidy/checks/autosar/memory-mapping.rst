.. title:: clang-tidy - autosar-memory-mapping

autosar-memory-mapping
======================

.. contents::

Overview
--------

`AUTOSAR <https://www.autosar.org/>`_ demands that every declaration of a
function and/or variable shall be explicitly memory mapped
(see *[SWS_MemMap_00003]* in [1]_). As the used method for doing this is quiet
unusual for most projects this approach is quiet error prone and easily
forgotten in general. Resulting errors can be caught earliest during linking
which makes it almost impossible to come up with an on the fly analysis for this
approach. Together with `clangd <https://clangd.llvm.org/>`_ this
`clang-tidy <../../index.html>`_ check may help to resolve a lot of the above
pain points as early as possible.


Below you can find *some* of the issues detected by the
`autosar-memory-mapping <memory-mapping.html>`_ check.

.. code-block:: c

   uint32 Swc_Variable; // unmapped variable declaration

   void Swc_Main(void) {} // unmapped function declaration

   ...

   #define Swc_START_SEC_CODE
   #include "Swc_MemMap.h"

   uint32 Swc_Variable; // variable cannot be mapped to a code section

   ...

   #define Swc_START_SEC_VAR_CLEARED_UNSPECIFIED
   #include "Swc_MemMap.h"

   uint32 Swc_Variable;

   #define Swc_STOP_SEC_CONST_UNSPECIFIED // non-matching stop symbol
   #include "Swc_MemMap.h"

   ...

   #define Swc_START_SEC_VAR_CLEARED_UNSPECIFIED
   // missing inclusion of memory mapping header

   uint32 Swc_Variable;

   ...

   #define Swc_START_SEC_VAR_CLEARED_8
   #include "Swc_MemMap.h"

   extern uint8 Swc_Variable;

   #define Swc_STOP_SEC_VAR_CLEARED_8
   #include "Swc_MemMap.h"

   #define Swc_START_SEC_VAR_CLEARED_32
   #include "Swc_MemMap.h"

   uint8 Swc_Variable = 0; // different section used than for declaration

   #define Swc_STOP_SEC_VAR_CLEARED_32
   #include "Swc_MemMap.h"


Options
-------

This section briefly describes all available options for
`autosar-memory-mapping <memory-mapping.html>`_.


.. option:: MemMapInclusion

    A **semicolon-separated glob pattern list** used to identify memory
    mapping header files. Usually these files can be
    indentified via the pattern "*\*_MemMap.h*" (see *[SWS_MemMap_00032]* and
    *[SWS_MemMap_00029]* in [1]_). Notice that the specified patterns will be
    applied on the **include path** of any included file.

.. Option:: SectionCode

   A **semicolon-separated list** of strings specifying code section types.
   Default is ``CODE``.

.. Option:: SectionConst

   A **semicolon-separated list** of strings specifying section types for
   constant data. Default is ``CONST``.

.. Option:: SectionVar

   A **semicolon-separated list** of strings specifying section types for
   variables. Default is ``VAR``.

.. Option:: PolicyCleared

   A **semicolon-separated list** of strings specifying sections that only
   allow for implicitly initialized constants and variables. Default is
   ``CLEARED;POWER_ON_CLEARED``.

.. Option:: PolicyInit

   A **semicolon-separated list** of strings specifying sections that only
   allow for explicitly initialized constants and variables. Default is
   ``INIT``.

.. Option:: AlignBoolean

   A **semicolon-separated list** of strings specifying the keyword alignment
   specification for 1-bit alignment. Default is ``BOOLEAN``.

.. Option:: Align8

   A **semicolon-separated list** of strings specifying the keyword alignment
   specification for 8-bit alignment. Default is ``8``.

.. Option:: Align16

   A **semicolon-separated list** of strings specifying the keyword alignment
   specification for 16-bit alignment. Default is ``16``.

.. Option:: Align32

   A **semicolon-separated list** of strings specifying the keyword alignment
   specification for 32-bit alignment. Default is ``32``.

.. Option:: Align64

   A **semicolon-separated list** of strings specifying the keyword alignment
   specification for 64-bit alignment. Default is ``64``.

.. Option:: AlignPointer

   A **semicolon-separated list** of strings specifying the keyword alignment
   specification for pointer alignment. Default is ``PTR``.

.. Option:: AlignUnspecified

   A **semicolon-separated list** of strings specifying the keyword alignment
   specification for unspecified alignment. Default is ``UNSPECIFIED``.

.. option:: KeywordStartSection

    A **semicolon-separated list** of strings specifying representing all
    possible start affixes within a memory mapping keyword.
    Defaults to ``_START_SEC_``. See *[SWS_MemMap_00022]* in [1]_ about the
    structure of memory mapping keywords keywords. Note that for every start
    affix, there has to exist a matching stop affix such that matching start and
    stop keywords compare equal besides the specified affixes themselves.

.. option:: KeywordStopSection

   See above documentation for option
   `KeywordStartSection <memory-mapping.html#cmdoption-arg-KeywordStartSection>`_.
   The value for this option defaults to ``_STOP_SEC_``.

.. option:: AllowEmptySections

    A **boolean value** used to enable or disable warnings about empty
    memory mapping sections.

.. option:: AllowRedundantSectionBreaks

   A **boolean value** used to enable or disable warnings about redundant
   breaks in memory mapping sections.

.. option:: AllowStaticLocalVariables

    A **boolean value** to disable warnings about inclusions of memory mapping
    header files inside function bodies making it essentially impossible to
    correctly memory map static local variables. Such warnings
    are emitted by this check to support *[SWS_MemMap_00023]* in [1]_.

    Below is a code example to demonstrate how static variables are declared
    within functions.

    .. code-block:: c

       #define Swc_START_SEC_CODE
       #include "Swc_MemMap.h"

       void Swc_Main(void)
       {
       #define Swc_STOP_SEC_CODE
       #include "Swc_MemMap.h" // no warning

       #define Swc_START_SEC_VAR
       #include "Swc_MemMap.h" // no warning
           static uint8 Flag = 0;
       #define Swc_STOP_SEC_VAR
       #include "Swc_MemMap.h" // no warning

       #define Swc_START_SEC_CODE
       #include "Swc_MemMap.h" // no warning
           ...
       }
       ...


.. option:: ApplyClangdAdjustments

   A **boolean value** which can be used to disable an additional preprocessing
   step automatically performed by this check if it encounters an active
   preamble. The additional preprocessing step eases using this check in
   conjunction with `clangd <https://clangd.llvm.org/>`_ but sometimes it might
   be favorable to disable it.

.. option:: StrictAlignmentSpecifications

   A **boolean value** which can be used to enable warnings about missing or
   invalid alignment specifications in constant data section keywords.
   By default, such keywords are allowed to omit the alignment specification.


Usage with clangd
-----------------

Without further adjustments, this check will report false positives at the top
of each file when used with `clangd <https://clangd.llvm.org/>`_.
This is due to the heavy reliance on the C preprocessor by the AUTOSAR memory
mapping construct and **clangd's preamble** optimization. There are two
independent options to deal with this issue.

#. On detecting an active preamble optimization usually done by clangd,
   the check automatically applies adjustments (see `ApplyClangdAdjustments
   <memory-mapping.html#cmdoption-arg-ApplyClangdAdjustments>`_)
   to prevent reporting false positive diagnostic messages. This should be
   sufficient for the overwhelming majority of cases.

#. To avoid false positive diagnostic messages by
   `clangd <https://clangd.llvm.org/>`_, its **preamble** can be stopped
   manually at a suitable source location. One way to achieve this is to add a
   macro definition like ``-DCLANGD_PREAMBLE_END=`` as a command-line
   argument to the invocation of the preprocessor and using this macro in
   source files like shown in the example below.

    .. code-block:: c

       #include "Rte_Swc.h"

       CLANGD_PREAMBLE_END

       #define Swc_START_SEC_VAR
       #include "Swc_MemMap.h"

       int Swc_Num = 0;

    In contrast to the previous mentioned option, this approach requires
    less CPU resources and is more reliabe. However, its major drawback
    is the manual intervention required to get it to work.

Please note that due to its aggressive preamble optimization strategy
and despite of the above mentioned adjustments,
`clangd <https://clangd.llvm.org/>`_ will still not be able to detect as many
memory mapping issues as its **clang-tidy** counterpart.

Limitations
-----------

The following limitations for the
`autosar-memory-mapping <memory-mapping.html>`_ check are currently known:

#. Although this check was designed to be tweaked via its configuration options
   to support as many AUTOSAR projects as possible, there are for sure some
   limitations. Especially tweaking the options
   `KeywordStartSection <memory-mapping.html#cmdoption-arg-KeywordStartSection>`_
   and
   `KeywordStopSection <memory-mapping.html#cmdoption-arg-KeywordStopSection>`_
   might get problematic. The following general rule applies:
   The less an AUTOSAR project and all its code suppliers deviate from the
   official AUTOSAR memory mapping specification [1]_ and the less tweaking
   therefore this **clang-tidy** check needs, the likelier it is for this check
   to perform well.

#. For various reasons, this check was specifically designed to work with
   emptied out memory mapping header files. It is therefore recommended
   to **always use emptied out memory mapping header files** or this check might
   become unusable.

#. Some projects might make use of **forwarding memory mapping header files**.
   Such header files contain only an inclusion directive of one ore more other
   memory mapping header files. It needs to be ensured that these **forwarding
   memory mapping header files** are treated exactly in the same way as normal
   memory mapping header files. In particular, this means they need to be
   **emptied out** and they need to match one of the globs defined in option
   `MemMapInclusion <memory-mapping.html#cmdoption-arg-MemMapInclusion>`_
   when using the `autosar-memory-mapping <memory-mapping.html>`_ check.


Rationale
---------

Some translation units within `AUTOSAR <https://www.autosar.org/>`_ projects
might become huge due to excessive use of the preprocessor by its memory mapping
mechanism. In fact, some translation units become too big for clang to handle
at all and thus resulting in the message *"fatal error: sorry, this include
generates a translation unit too large for Clang to process."*. One way to work
around this issue is to use a virtual filesystem for stubbing out all memory
mapping header files. However, this results in loss of some of the information
and some builtin error detection mechanisms (e.g. see *[SWS_MemMap_00007]* in
[1]_). Despite of this, the check described here is able to detect a wide
variety of issues related to the AUTOSAR memory mapping mechanism  while still
being suitable for on the fly analysis via `clangd <https://clangd.llvm.org/>`_
and thus effectively helping to improve code quality as soon as possible.


References
----------

.. [1]
   `Specification of Memory Mapping - AUTOSAR CP R22-11
   <https://www.autosar.org/fileadmin/standards/R22-11/CP/AUTOSAR_SWS_MemoryMapping.pdf>`_

