#include "ClangTidyTest.h"
#include "gtest/gtest.h"

// Disable the hidden keyword warnings to make testing simpler and keep the
// build output clean.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wkeyword-macro"

#define protected public
#define private public

#include "misrac/EssentialTypeCheckBase.h"

#undef private
#undef protected

#pragma GCC diagnostic pop

using namespace clang::tidy::misrac;

namespace clang {
namespace tidy {
namespace test {

static ClangTidyContext createClangTidyContext(ClangTidyOptions Options) {
  auto Provider = std::make_unique<DefaultOptionsProvider>(
      ClangTidyGlobalOptions(), std::move(Options));

  return ClangTidyContext(std::move(Provider));
}

static EssentialTypeCheckBase
createEssentialTypeVisitor(ClangTidyContext *Context) {
  return EssentialTypeCheckBase("", Context);
}

TEST(EssentialTypeVisitor, ConfigBooleanTypes01) {
  ClangTidyOptions Options;
  Options.CheckOptions["misrac.shared-options.TreatAsBoolean"] = "";

  auto Context = createClangTidyContext(std::move(Options));
  auto Visitor = createEssentialTypeVisitor(&Context);

  ASSERT_FALSE(Visitor.isBooleanType("a"));
  ASSERT_FALSE(Visitor.isBooleanType("b"));
}

TEST(EssentialTypeVisitor, ConfigBooleanTypes02) {
  ClangTidyOptions Options;
  Options.CheckOptions["misrac.shared-options.TreatAsBoolean"] = "a";

  auto Context = createClangTidyContext(std::move(Options));
  auto Visitor = createEssentialTypeVisitor(&Context);

  ASSERT_TRUE(Visitor.isBooleanType("a"));
}

TEST(EssentialTypeVisitor, ConfigBooleanTypes03) {
  ClangTidyOptions Options;
  Options.CheckOptions["misrac.shared-options.TreatAsBoolean"] = "a;b;c";

  auto Context = createClangTidyContext(std::move(Options));
  auto Visitor = createEssentialTypeVisitor(&Context);

  ASSERT_TRUE(Visitor.isBooleanType("a"));
  ASSERT_TRUE(Visitor.isBooleanType("b"));
  ASSERT_TRUE(Visitor.isBooleanType("c"));
}

TEST(EssentialTypeVisitor, ConfigBooleanTypes04) {
  ClangTidyOptions Options;
  Options.CheckOptions["misrac.shared-options.TreatAsBoolean"] = " a; b ;c ";

  auto Context = createClangTidyContext(std::move(Options));
  auto Visitor = createEssentialTypeVisitor(&Context);

  ASSERT_TRUE(Visitor.isBooleanType("a"));
  ASSERT_TRUE(Visitor.isBooleanType("b"));
  ASSERT_TRUE(Visitor.isBooleanType("c"));
}

} // namespace test
} // namespace tidy
} // namespace clang
