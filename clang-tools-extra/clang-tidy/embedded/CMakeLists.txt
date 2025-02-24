set(LLVM_LINK_COMPONENTS
  support
  FrontendOpenMP
  )

add_clang_library(clangTidyEmbeddedModule
  EmbeddedTidyModule.cpp
  MissingCompoundCheck.cpp
  MissingElseCheck.cpp
  NoCommaOperatorCheck.cpp
  NoEarlyReturnCheck.cpp
  NoUnionCheck.cpp

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
