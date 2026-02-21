set(LLVM_LINK_COMPONENTS
  support
  FrontendOpenMP
  )

add_clang_library(clangTidyMisracModule
  MisracTidyModule.cpp
  EssentialTypeCheckBase.cpp
  Rule11p1Check.cpp
  Rule11p5Check.cpp
  Rule11p8Check.cpp
  Rule11p9Check.cpp
  Rule12p1Check.cpp
  Rule15p4Check.cpp
  Rule15p6Check.cpp
  Rule16p3Check.cpp
  Rule17p1Check.cpp
  Rule17p8Check.cpp
  Rule20p2Check.cpp
  Rule20p4Check.cpp
  Rule8p12Check.cpp
  Rule8p3Check.cpp
  Rule8p4Check.cpp
  Rule8p8Check.cpp

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
