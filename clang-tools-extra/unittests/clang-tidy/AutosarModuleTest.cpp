#include "ClangTidyTest.h"
#include "autosar/MemoryMappingCheck.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "gtest/gtest.h"

using namespace clang::tidy::autosar;

namespace clang {
namespace tidy {
namespace test {

static ClangTidyContext createClangTidyContext(ClangTidyOptions Options) {
  auto Provider = std::make_unique<DefaultOptionsProvider>(
      ClangTidyGlobalOptions(), std::move(Options));

  return ClangTidyContext(std::move(Provider));
}

static MemoryMappingCheck createMemoryMappingCheck(ClangTidyContext *Context) {
  return MemoryMappingCheck("autosar-memory-mapping", Context);
}

TEST(MemoryMappingCheckTest, InitMemMapInclusions) {
  ClangTidyOptions Options;
  Options.CheckOptions["autosar-memory-mapping.MemMapInclusion"] =
      "A;*B;C*;*D*;E";

  auto Context = createClangTidyContext(std::move(Options));
  auto Check = createMemoryMappingCheck(&Context);

  ASSERT_FALSE(Check.isMemMapFile("Swc_MemMap.h"));
  ASSERT_FALSE(Check.isMemMapFile("F"));

  ASSERT_TRUE(Check.isMemMapFile("A"));
  ASSERT_TRUE(Check.isMemMapFile("B"));
  ASSERT_TRUE(Check.isMemMapFile("AB"));
  ASSERT_TRUE(Check.isMemMapFile("C"));
  ASSERT_TRUE(Check.isMemMapFile("CE"));
  ASSERT_TRUE(Check.isMemMapFile("ADC"));
  ASSERT_TRUE(Check.isMemMapFile("E"));
}

TEST(MemoryMappingCheckTest, CheckOptionsSection) {
  ClangTidyOptions Options;
  Options.CheckOptions["autosar-memory-mapping.SectionCode"] = "A;CCC";
  Options.CheckOptions["autosar-memory-mapping.SectionConst"] =
      "BB;FFFFFF;GGGGGGG";
  Options.CheckOptions["autosar-memory-mapping.SectionVar"] = "DDDD;EEEEE";

  auto Context = createClangTidyContext(std::move(Options));
  auto Check = createMemoryMappingCheck(&Context);

  auto Sections = Check.getSectionTypes();

  ASSERT_EQ(7u, Sections.size());

  // Array must be sorted by the length of its elements
  ASSERT_EQ("GGGGGGG", Sections[0].first);
  ASSERT_EQ(Keyword::SectionType::CONST, Sections[0].second);

  ASSERT_EQ("FFFFFF", Sections[1].first);
  ASSERT_EQ(Keyword::SectionType::CONST, Sections[1].second);

  ASSERT_EQ("EEEEE", Sections[2].first);
  ASSERT_EQ(Keyword::SectionType::VAR, Sections[2].second);

  ASSERT_EQ("DDDD", Sections[3].first);
  ASSERT_EQ(Keyword::SectionType::VAR, Sections[3].second);

  ASSERT_EQ("CCC", Sections[4].first);
  ASSERT_EQ(Keyword::SectionType::CODE, Sections[4].second);

  ASSERT_EQ("BB", Sections[5].first);
  ASSERT_EQ(Keyword::SectionType::CONST, Sections[5].second);

  ASSERT_EQ("A", Sections[6].first);
  ASSERT_EQ(Keyword::SectionType::CODE, Sections[6].second);
}

TEST(MemoryMappingCheckTest, CheckOptionsInitPolicy) {
  ClangTidyOptions Options;
  Options.CheckOptions["autosar-memory-mapping.PolicyCleared"] =
      "BB;CCC;FFFFFF";
  Options.CheckOptions["autosar-memory-mapping.PolicyInit"] =
      "A;DDDD;EEEEE";

  auto Context = createClangTidyContext(std::move(Options));
  auto Check = createMemoryMappingCheck(&Context);

  auto InitPolicies = Check.getInitPolicies();

  ASSERT_EQ(6u, InitPolicies.size());

  // Array must be sorted by the length of its elements
  ASSERT_EQ("FFFFFF", InitPolicies[0].first);
  ASSERT_EQ(Keyword::InitPolicy::CLEARED, InitPolicies[0].second);

  ASSERT_EQ("EEEEE", InitPolicies[1].first);
  ASSERT_EQ(Keyword::InitPolicy::INIT, InitPolicies[1].second);

  ASSERT_EQ("DDDD", InitPolicies[2].first);
  ASSERT_EQ(Keyword::InitPolicy::INIT, InitPolicies[2].second);

  ASSERT_EQ("CCC", InitPolicies[3].first);
  ASSERT_EQ(Keyword::InitPolicy::CLEARED, InitPolicies[3].second);

  ASSERT_EQ("BB", InitPolicies[4].first);
  ASSERT_EQ(Keyword::InitPolicy::CLEARED, InitPolicies[4].second);

  ASSERT_EQ("A", InitPolicies[5].first);
  ASSERT_EQ(Keyword::InitPolicy::INIT, InitPolicies[5].second);
}

TEST(MemoryMappingCheckTest, CheckOptionsAlign) {
  ClangTidyOptions Options;
  Options.CheckOptions["autosar-memory-mapping.AlignBoolean"] = "CCC;EEEEE";
  Options.CheckOptions["autosar-memory-mapping.Align8"] = "FFFFFF";
  Options.CheckOptions["autosar-memory-mapping.Align16"] = "HHHHHHHH";
  Options.CheckOptions["autosar-memory-mapping.Align32"] = "DDDD";
  Options.CheckOptions["autosar-memory-mapping.Align64"] = "A";
  Options.CheckOptions["autosar-memory-mapping.AlignPointer"] = "GGGGGGG";
  Options.CheckOptions["autosar-memory-mapping.AlignUnspecified"] = "BB";

  auto Context = createClangTidyContext(std::move(Options));
  auto Check = createMemoryMappingCheck(&Context);

  auto Specs = Check.getAlignmentSpecs();

  ASSERT_EQ(8u, Specs.size());

  // Array must be sorted by the length of its elements
  ASSERT_EQ("HHHHHHHH", Specs[0].first);
  ASSERT_EQ(Keyword::AlignSpec::_16, Specs[0].second);

  ASSERT_EQ("GGGGGGG", Specs[1].first);
  ASSERT_EQ(Keyword::AlignSpec::PTR, Specs[1].second);

  ASSERT_EQ("FFFFFF", Specs[2].first);
  ASSERT_EQ(Keyword::AlignSpec::_8, Specs[2].second);

  ASSERT_EQ("EEEEE", Specs[3].first);
  ASSERT_EQ(Keyword::AlignSpec::BOOLEAN, Specs[3].second);

  ASSERT_EQ("DDDD", Specs[4].first);
  ASSERT_EQ(Keyword::AlignSpec::_32, Specs[4].second);

  ASSERT_EQ("CCC", Specs[5].first);
  ASSERT_EQ(Keyword::AlignSpec::BOOLEAN, Specs[5].second);

  ASSERT_EQ("BB", Specs[6].first);
  ASSERT_EQ(Keyword::AlignSpec::UNSPECIFIED, Specs[6].second);

  ASSERT_EQ("A", Specs[7].first);
  ASSERT_EQ(Keyword::AlignSpec::_64, Specs[7].second);
}

class ClangdAdjustmentsTestFixture {
public:
  ClangdAdjustmentsTestFixture()
      : FileName("Swc.c"), Code(),
        VirtualFS(new llvm::vfs::InMemoryFileSystem()) {

    Code = R"(
#include "Rte_Swc.h"

#define Swc_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"

static int Swc_Var;

#define Swc_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Swc_MemMap.h"
)";

    VirtualFS->addFile("Rte_Swc.h", 0, llvm::MemoryBuffer::getMemBuffer(""));
    VirtualFS->addFile("Swc_MemMap.h", 0, llvm::MemoryBuffer::getMemBuffer(""));
    VirtualFS->addFile(FileName, 0, llvm::MemoryBuffer::getMemBuffer(Code));
  }

  bool runTest(std::vector<ClangTidyError> *Diagnostics, bool ApplyAdjustments,
               bool UsePreamble) {
    // This class is just a convenient way to get a properly initialized
    // SourceManager and Preprocessor instance. Nothing gets actually parsed
    // due to the InitOnlyAction.
    class ClangdAdjustmentsTestAction : public InitOnlyAction {
    public:
      ClangdAdjustmentsTestAction(ClangTidyContext *Context,
                                  ClangTidyCheck *Check,
                                  DiagnosticsEngine *DiagEngine,
                                  bool UsePreamble)
          : Context(Context), Check(Check), DiagEngine(DiagEngine),
            UsePreamble(UsePreamble) {}

    protected:
      bool BeginSourceFileAction(CompilerInstance &CI) override {
        auto MainFile = getCurrentFile();
        auto &SM = CI.getSourceManager();
        auto &PP = CI.getPreprocessor();
        auto &PPOpts = PP.getPreprocessorOpts();

        Context->setCurrentFile(MainFile);
        DiagEngine->setSourceManager(&SM);

        if (UsePreamble) {
          // Simulate running in a clangd context
          auto &LangOpts = CI.getLangOpts();

          auto Buffer = SM.getBufferDataOrNone(SM.getMainFileID()).value();
          auto [Size, NewLine] = Lexer::ComputePreamble(Buffer, LangOpts);

          PPOpts.PrecompiledPreambleBytes.first = Size;
          PPOpts.PrecompiledPreambleBytes.second = NewLine;
        }

        // Warning: The lifetime of 'Check' is bound to this function, while
        // the Preprocessor instance lives for the duration of this
        // FrontendAction.
        Check->registerPPCallbacks(SM, &PP, &PP);

        // Generate diagnostic messages
        Check->onEndOfTranslationUnit();

        return true;
      }

    private:
      ClangTidyContext *Context;
      ClangTidyCheck *Check;
      DiagnosticsEngine *DiagEngine;
      bool UsePreamble;
    };

    ClangTidyOptions CTOpts;
    CTOpts.Checks = "autosar-memory-mapping";
    CTOpts.CheckOptions["autosar-memory-mapping.ApplyClangdAdjustments"] =
        (ApplyAdjustments) ? "true" : "false";

    auto Context = createClangTidyContext(std::move(CTOpts));
    auto Check = createMemoryMappingCheck(&Context);

    ClangTidyDiagnosticConsumer DiagConsumer(Context);
    DiagnosticsEngine DiagEngine(new clang::DiagnosticIDs(),
                                 new DiagnosticOptions(), &DiagConsumer, false);

    Context.setDiagnosticsEngine(&DiagEngine);

    auto Action = std::make_unique<ClangdAdjustmentsTestAction>(
        &Context, &Check, &DiagEngine, UsePreamble);

    std::vector<std::string> Args;
    bool Ok = tooling::runToolOnCodeWithArgs(std::move(Action), Code, VirtualFS,
                                             Args, FileName);

    *Diagnostics = DiagConsumer.take();

    return Ok;
  }

private:
  StringRef FileName;
  StringRef Code;
  IntrusiveRefCntPtr<llvm::vfs::InMemoryFileSystem> VirtualFS;
};

TEST(MemoryMappingCheckTest, ClangdAdjustments01) {
  std::vector<ClangTidyError> Diagnostics;
  bool ApplyAdjustments = false;
  bool UsePreamble = false;

  auto Fixture = ClangdAdjustmentsTestFixture();
  auto Ok = Fixture.runTest(&Diagnostics, ApplyAdjustments, UsePreamble);

  ASSERT_TRUE(Ok);
  ASSERT_EQ(0u, Diagnostics.size());
}

TEST(MemoryMappingCheckTest, ClangdAdjustments02) {
  std::vector<ClangTidyError> Diagnostics;
  bool ApplyAdjustments = false;
  bool UsePreamble = true;

  auto Fixture = ClangdAdjustmentsTestFixture();
  auto Ok = Fixture.runTest(&Diagnostics, ApplyAdjustments, UsePreamble);

  ASSERT_TRUE(Ok);
  ASSERT_EQ(0u, Diagnostics.size());
}

TEST(MemoryMappingCheckTest, ClangdAdjustments03) {
  std::vector<ClangTidyError> Diagnostics;
  bool ApplyAdjustments = true;
  bool UsePreamble = false;

  auto Fixture = ClangdAdjustmentsTestFixture();
  auto Ok = Fixture.runTest(&Diagnostics, ApplyAdjustments, UsePreamble);

  ASSERT_TRUE(Ok);
  ASSERT_EQ(0u, Diagnostics.size());
}

TEST(MemoryMappingCheckTest, ClangdAdjustments04) {
  std::vector<ClangTidyError> Diagnostics;
  bool ApplyAdjustments = true;
  bool UsePreamble = true;

  auto Fixture = ClangdAdjustmentsTestFixture();
  auto Ok = Fixture.runTest(&Diagnostics, ApplyAdjustments, UsePreamble);

  ASSERT_TRUE(Ok);
  ASSERT_EQ(1u, Diagnostics.size());
  ASSERT_EQ("autosar-memory-mapping", Diagnostics[0].DiagnosticName);
  ASSERT_EQ("memory mapping section started by "
            "'Swc_START_SEC_VAR_CLEARED_UNSPECIFIED' not stopped",
            Diagnostics[0].Message.Message);
}

} // namespace test
} // namespace tidy
} // namespace clang
