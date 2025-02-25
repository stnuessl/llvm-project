set(LLVM_LINK_COMPONENTS
  support
  FrontendOpenMP
  )

add_clang_library(clangTidyEmbeddedModule
  DefaultCasePositionCheck.cpp
  EmbeddedTidyModule.cpp
  MissingCompoundCheck.cpp
  MissingDefaultLabelCheck.cpp
  MissingElseCheck.cpp
  MissingStaticCheck.cpp
  NoCommaOperatorCheck.cpp
  NoEarlyReturnCheck.cpp
  NoFlexibleArrayMemberCheck.cpp
  NoPartialArrayInitCheck.cpp
  NoStdargFeaturesCheck.cpp
  NoUnionCheck.cpp
  NoVariableLengthArrayCheck.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  )

clang_target_link_libraries(clangTidyEmbeddedModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTooling
  clangTransformer
  )
