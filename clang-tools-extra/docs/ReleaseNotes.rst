====================================================
Extra Clang Tools |release| |ReleaseNotesTitle|
====================================================

.. contents::
   :local:
   :depth: 3

Written by the `LLVM Team <https://llvm.org/>`_

.. only:: PreRelease

  .. warning::
     These are in-progress notes for the upcoming Extra Clang Tools |version| release.
     Release notes for previous releases can be found on
     `the Download Page <https://releases.llvm.org/download.html>`_.

Introduction
============

This document contains the release notes for the Extra Clang Tools, part of the
Clang release |release|. Here we describe the status of the Extra Clang Tools in
some detail, including major improvements from the previous release and new
feature work. All LLVM releases may be downloaded from the `LLVM releases web
site <https://llvm.org/releases/>`_.

For more information about Clang or LLVM, including information about
the latest release, please see the `Clang Web Site <https://clang.llvm.org>`_ or
the `LLVM Web Site <https://llvm.org>`_.

Note that if you are reading this file from a Git checkout or the
main Clang web page, this document applies to the *next* release, not
the current one. To see the release notes for a specific release, please
see the `releases page <https://llvm.org/releases/>`_.

What's New in Extra Clang Tools |release|?
==========================================

Some of the major new features and improvements to Extra Clang Tools are listed
here. Generic improvements to Extra Clang Tools as a whole or to its underlying
infrastructure are described first, followed by tool-specific sections.

Major New Features
------------------

...

Improvements to clangd
----------------------

Inlay hints
^^^^^^^^^^^

Diagnostics
^^^^^^^^^^^

Semantic Highlighting
^^^^^^^^^^^^^^^^^^^^^

Compile flags
^^^^^^^^^^^^^

Hover
^^^^^

Code completion
^^^^^^^^^^^^^^^

Code actions
^^^^^^^^^^^^

Signature help
^^^^^^^^^^^^^^

Cross-references
^^^^^^^^^^^^^^^^

Objective-C
^^^^^^^^^^^

Miscellaneous
^^^^^^^^^^^^^

Improvements to clang-doc
-------------------------

Improvements to clang-query
---------------------------

Improvements to clang-rename
----------------------------

The improvements are...

Improvements to clang-tidy
--------------------------

New checks
^^^^^^^^^^

- New :doc:`autosar-interface-connected
  <clang-tidy/checks/autosar/interface-connected>` check.

  Checks automatically generated AUTOSAR interfaces and emits appropriate
  diagnostics if unconnected interfaces are detected.

- New :doc:`autosar-memory-mapping
  <clang-tidy/checks/autosar/memory-mapping>` check.

  Detects multiple potential issues with regards to the AUTOSAR memory mapping
  specification including unmapped or wrongly mapped variables and functions.

- New :doc:`autosar-type-usage
  <clang-tidy/checks/autosar/type-usage>` check.

  Detects issues with regards to AUTOSAR types and their associated macro values
  by essentially treating these types as enumerations.

- New :doc:`embedded-default-case-position
  <clang-tidy/checks/embedded/default-case-position>` check.

  Detects default statements within switch statements that are not located at
  the first or last position in the case list.

- New :doc:`embedded-missing-compound
  <clang-tidy/checks/embedded/missing-compound>` check.

  Detects `do`, `for`, `while`, `if`, `else`, and `switch` statements where the
  body is not a compound statement.

- New :doc:`embedded-missing-else
  <clang-tidy/checks/embedded/missing-else>` check.

  Detects `if` ... `else if` constructs that lack a terminating `else`
  statement. This pattern may be interpreted similarly to a `switch`
  statement without a `default` case.

- New :doc:`embedded-missing-static
  <clang-tidy/checks/embedded/missing-static>` check.

  Detects inline function declarations that lack the static storage qualifier.

- New :doc:`embedded-no-comma-operator
  <clang-tidy/checks/embedded/no-comma-operator>` check.

  Detects usage of the comma operator, which is considered bad practice
  according to coding standards like `MISRA C <https://misra.org.uk>`_.

- New :doc:`embedded-no-early-return
  <clang-tidy/checks/embedded/no-early-return>` check.

  Detects early return statements that are not placed at the end of
  a function body’s compound statement.

- New :doc:`embedded-no-flexible-array-member
  <clang-tidy/checks/embedded/no-flexible-array-member>` check.

  Detects flexible array member declarations.

- New :doc:`embedded-no-partial-array-init
  <clang-tidy/checks/embedded/no-partial-array-init>` check.

  Detects partial array initializiations.

- New :doc:`embedded-no-stdarg-features
  <clang-tidy/checks/embedded/no-stdarg-features>` check.

  Detects usage of types and macros from `stdarg.h`.

- New :doc:`embedded-no-union
  <clang-tidy/checks/embedded/no-union>` check.

  Detects union declarations, which are considered bad practice according to
  coding standards like `MISRA C <https://misra.org.uk>`_.

- New :doc:`embedded-no-variable-length-array
  <clang-tidy/checks/embedded/no-variable-length-array>` check.

  Detects declarations of variable length arrays.

New check aliases
^^^^^^^^^^^^^^^^^

Changes in existing checks
^^^^^^^^^^^^^^^^^^^^^^^^^^

Removed checks
^^^^^^^^^^^^^^

Miscellaneous
^^^^^^^^^^^^^

Improvements to include-fixer
-----------------------------

The improvements are...

Improvements to clang-include-fixer
-----------------------------------

The improvements are...

Improvements to modularize
--------------------------

The improvements are...

Improvements to pp-trace
------------------------

Clang-tidy Visual Studio plugin
-------------------------------
