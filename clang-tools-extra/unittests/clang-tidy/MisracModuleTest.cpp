#include "ClangTidyTest.h"
#include "misrac/Rule20p2Check.h"
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

TEST(MisracRule20p2, DefaultConfig01) {
  auto Code = StringRef(R"(
#include "file'h"
#include <"file.h">
#include "dir//file.h"

)");

  std::vector<ClangTidyError> Errors;
  auto PathsToContent = std::map<StringRef, StringRef>({
      {"file'h", ""},
      {"\"file.h\"", ""},
      {"dir//file.h", ""},
  });

  ASSERT_EQ(Code, runCheckOnCode<misrac::Rule20p2Check>(
                      Code, &Errors, "input.cc", {}, ClangTidyOptions(),
                      PathsToContent));

  ASSERT_EQ(4u, Errors.size());

  ASSERT_EQ("invalid character '''", Errors[0].Message.Message);
  ASSERT_EQ("invalid character '\"'", Errors[1].Message.Message);
  ASSERT_EQ("invalid character '\"'", Errors[2].Message.Message);
  ASSERT_EQ("invalid character sequence '//'", Errors[3].Message.Message);

  ASSERT_EQ(15u, Errors[0].Message.FileOffset);
  ASSERT_EQ(29u, Errors[1].Message.FileOffset);
  ASSERT_EQ(36u, Errors[2].Message.FileOffset);
  ASSERT_EQ(52u, Errors[3].Message.FileOffset);

  ASSERT_EQ(52u, Errors[3].Message.Ranges[0].FileOffset);
  ASSERT_EQ(2u, Errors[3].Message.Ranges[0].Length);
}

TEST(MisracRule20p2, Config01) {
  auto Code = StringRef(R"(
#include "file'h"
#include "file?h"
#include "file h"

)");

  ClangTidyOptions Options;
  Options.CheckOptions["test-check-0.Regex"] = R"([\? ])";

  std::vector<ClangTidyError> Errors;
  auto PathsToContent = std::map<StringRef, StringRef>({
      {"file'h", ""},
      {"file?h", ""},
      {"file h", ""},
  });

  ASSERT_EQ(Code, runCheckOnCode<misrac::Rule20p2Check>(
                      Code, &Errors, "input.cc", {}, Options, PathsToContent));

  ASSERT_EQ(3u, Errors.size());

  ASSERT_EQ("invalid character '''", Errors[0].Message.Message);
  ASSERT_EQ("invalid character '?'", Errors[1].Message.Message);
  ASSERT_EQ("invalid character ' '", Errors[2].Message.Message);

  ASSERT_EQ(15u, Errors[0].Message.FileOffset);
  ASSERT_EQ(33u, Errors[1].Message.FileOffset);
  ASSERT_EQ(51u, Errors[2].Message.FileOffset);
}

} // namespace test
} // namespace tidy
} // namespace clang
