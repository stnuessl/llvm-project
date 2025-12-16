set(LLVM_LINK_COMPONENTS
  support
  FrontendOpenMP
  )

add_clang_library(clangTidyMisracModule
  MisracTidyModule.cpp
  EssentialTypeCheckBase.cpp
  Rule15p6Check.cpp
  Rule16p3Check.cpp
  Rule17p1Check.cpp
  Rule17p8Check.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  )

clang_target_link_libraries(clangTidyMisracModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTooling
  clangTransformer
  )
