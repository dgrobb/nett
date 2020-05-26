/*  Copyright (C) 2020  Matthys Grobbelaar
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "checks/braces/EnumBracesCheck.hpp"
#include "checks/braces/FunctionBodyBracesCheck.hpp"
#include "checks/braces/StructUnionBracesCheck.hpp"
#include "checks/comments/CommentManager.hpp"
#include "checks/comments/FunctionCommentCheck.hpp"
#include "checks/comments/GlobalVarCommentCheck.hpp"
#include "checks/indentation/EnumBodyIndentCheck.hpp"
#include "checks/indentation/FuncDeclParamContIndentCheck.hpp"
#include "checks/indentation/FunctionBodyIndentCheck.hpp"
#include "checks/indentation/GlobalVarIndentCheck.hpp"
#include "checks/indentation/StructUnionBodyIndentCheck.hpp"
#include "checks/indentation/TypedefIndentCheck.hpp"
#include "checks/naming/DefineNameCheck.hpp"
#include "checks/naming/EnumConstNameCheck.hpp"
#include "checks/naming/EnumNameCheck.hpp"
#include "checks/naming/FuncNameCheck.hpp"
#include "checks/naming/StructUnionFieldNameCheck.hpp"
#include "checks/naming/StructUnionNameCheck.hpp"
#include "checks/naming/TypedefNameCheck.hpp"
#include "checks/naming/VarNameCheck.hpp"
#include "checks/overall/FuncDefinitionInHeaderCheck.hpp"
#include "checks/overall/FuncLengthCheck.hpp"
#include "checks/overall/KeywordRedefCheck.hpp"
#include "checks/warning/GoToCheck.hpp"
#include "checks/whitespace/BinaryOperatorWhitespaceCheck.hpp"
#include "checks/whitespace/EnumWhitespaceCheck.hpp"
#include "checks/whitespace/FunctionBodyWhitespaceCheck.hpp"
#include "checks/whitespace/FunctionCallWhitespaceCheck.hpp"
#include "checks/whitespace/GlobalVariableSemicolonWhitespaceCheck.hpp"
#include "checks/whitespace/StatementSemicolonWhitespaceCheck.hpp"
#include "checks/whitespace/StructUnionMemberAccessWhitespaceCheck.hpp"
#include "checks/whitespace/StructUnionWhitespaceCheck.hpp"
#include "checks/whitespace/UnaryOperatorWhitespaceCheck.hpp"
#include "checks/whitespace/VarDeclWhitespaceCheck.hpp"
#include "checks/whitespace/TernaryOperatorWhitespaceCheck.hpp"
#include "input/FileInput.hpp"
#include "output/OutputColors.hpp"
#include "output/UserOutput.hpp"
#include "violations/ViolationManager.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;
using namespace llvm;
using namespace nett;

// COMMAND LINE SETUP ---------------------------------------------------------
static llvm::cl::OptionCategory NettOptionCategory("nett options");
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::opt<bool> GenerateFiles("generate-files",
        cl::desc(R"(Generate .styled files for each input file parsed instead
of printing the violations to the terminal. These files are 
duplicates of the original files, amended with the style 
violations detected within them.
)"),
        cl::init(false), cl::cat(NettOptionCategory));

static cl::opt<bool> ShowLicense("license",
        cl::desc(R"(Show the license for the software.
)"),
        cl::init(false), cl::cat(NettOptionCategory));

// SUPPRESSIONS ---------------------------------------------------------------

// The WarningDiagConsumer allows us to suppress warning and error messages
// which are raised when a file is being parsed by clang. We do nothing when
// handling a diagnostic since we are simply style checking code which we
// assume already compiles.
class WarningDiagConsumer : public DiagnosticConsumer {

    public:
    virtual void HandleDiagnostic(
            DiagnosticsEngine::Level Level, const Diagnostic& Info) override {
        // simply do nothing
    }
};

// MATCHERS -------------------------------------------------------------------

namespace {

AST_MATCHER(VarDecl, isLocalVariable) {
    return Node.isLocalVarDecl();
}

// Variable declaration matcher
auto VarDeclMatcher = varDecl().bind("varDecl");
// Function declaration matcher
auto FuncDeclMatcher = functionDecl().bind("funcDecl");
// Struct/Union declaration matcher
auto StructUnionDeclMatcher = recordDecl().bind("recordDecl");
// Struct/Union field matcher
auto StructUnionFieldMatcher = fieldDecl().bind("fieldDecl");
// Enum declaration matcher
auto EnumDeclMatcher = enumDecl().bind("enumDecl");
// Enum constant matcher
auto EnumConstMatcher = enumConstantDecl().bind("enumConstDecl");
// Typedef matcher
auto TypedefMatcher = typedefDecl().bind("typedefDecl");
// goto statement matcher
auto GoToMatcher = gotoStmt().bind("gotoStmt");
// Global variable matcher
auto GlobalVarMatcher = varDecl(hasGlobalStorage(), unless(isLocalVariable()),
        unless(matchesName("::g[A-Z]")))
                                .bind("globalVar");
// Top level struct/union matcher
auto TopLevelRecordMatcher = recordDecl(
        unless(hasAncestor(recordDecl())), unless(hasAncestor(functionDecl())))
                                     .bind("topLevelRecordDecl");
// Top level enum matcher
auto TopLevelEnumMatcher = enumDecl(
        unless(hasAncestor(recordDecl())), unless(hasAncestor(functionDecl())))
                                   .bind("topLevelEnumDecl");
// Top level typedef matcher
auto TopLevelTypedefMatcher = typedefDecl(
        unless(hasAncestor(recordDecl())), unless(hasAncestor(functionDecl())))
                                      .bind("topLevelTypedefDecl");
// Binary operator matcher
auto BinOpMatcher = binaryOperator().bind("binOp");

// Member expression (X->a / X.a) matcher
auto MemberExprMatcher = memberExpr().bind("memberExpr");

// Unary operator matcher
auto UnaryOpMatcher = unaryOperator().bind("unaryOp");

// Statement matcher
auto StmtMatcher = stmt().bind("stmt");

// Function call matcher
auto FuncCallMatcher = callExpr().bind("funcCall");

// Ternary expression matcher
auto TernaryMatcher = conditionalOperator().bind("ternaryExpr");

}  // namespace

// TRAVERSAL SETUP ------------------------------------------------------------

// The ASTConsumer allows us to dictate which AST nodes we match and how we
// want to handle those matches.
class NettASTConsumer : public ASTConsumer {

    public:
    NettASTConsumer(clang::Preprocessor& PP) {
        // Here we add all of the checks that should be run
        // when the AST is traversed by using Matcher.addMatcher

        // NAMING checks
        Matcher.addMatcher(
                VarDeclMatcher, new checks::naming::VarNameChecker());
        Matcher.addMatcher(
                FuncDeclMatcher, new checks::naming::FuncNameChecker());
        Matcher.addMatcher(StructUnionDeclMatcher,
                new checks::naming::RecordNameChecker());
        Matcher.addMatcher(StructUnionFieldMatcher,
                new checks::naming::RecordFieldNameChecker());
        Matcher.addMatcher(
                EnumDeclMatcher, new checks::naming::EnumNameChecker());
        Matcher.addMatcher(
                EnumConstMatcher, new checks::naming::EnumConstNameChecker());
        Matcher.addMatcher(
                TypedefMatcher, new checks::naming::TypedefNameChecker());

        // COMMENTS checks
        Matcher.addMatcher(GlobalVarMatcher,
                new checks::comments::GlobalVarCommentChecker());
        Matcher.addMatcher(FuncDeclMatcher,
                new checks::comments::FunctionCommentChecker());

        // BRACES checks
        Matcher.addMatcher(FuncDeclMatcher,
                new checks::braces::FunctionBodyBracesChecker());
        Matcher.addMatcher(
                EnumDeclMatcher, new checks::braces::EnumBracesChecker());
        Matcher.addMatcher(StructUnionDeclMatcher,
                new checks::braces::StructUnionBracesChecker());

        // WHITESPACE checks
        Matcher.addMatcher(BinOpMatcher,
                new checks::whitespace::BinaryOperatorWhitespaceChecker());
        Matcher.addMatcher(MemberExprMatcher,
                new checks::whitespace::
                        StructUnionMemberAccessWhitespaceChecker());
        Matcher.addMatcher(UnaryOpMatcher,
                new checks::whitespace::UnaryOperatorWhitespaceChecker());
        Matcher.addMatcher(StmtMatcher,
                new checks::whitespace::StatementSemicolonWhitespaceChecker());
        Matcher.addMatcher(GlobalVarMatcher,
                new checks::whitespace::
                        GlobalVariableSemicolonWhitespaceChecker());
        Matcher.addMatcher(FuncDeclMatcher,
                new checks::whitespace::FunctionBodyWhitespaceChecker(PP));
        Matcher.addMatcher(EnumDeclMatcher,
                new checks::whitespace::EnumWhitespaceChecker(PP));
        Matcher.addMatcher(StructUnionDeclMatcher,
                new checks::whitespace::StructUnionWhitespaceChecker(PP));
        Matcher.addMatcher(FuncCallMatcher,
                new checks::whitespace::FunctionCallWhitespaceChecker(PP));
        Matcher.addMatcher(VarDeclMatcher,
                new checks::whitespace::VarDeclWhitespaceChecker(PP));
        Matcher.addMatcher(TernaryMatcher,
                new checks::whitespace::TernaryOperatorWhitespaceChecker());

        // INDENTATION checks
        Matcher.addMatcher(FuncDeclMatcher,
                new checks::indentation::FuncDeclParamContIndentChecker());
        Matcher.addMatcher(FuncDeclMatcher,
                new checks::indentation::FunctionBodyIndentChecker(PP));
        Matcher.addMatcher(GlobalVarMatcher,
                new checks::indentation::GlobalVarIndentChecker());
        Matcher.addMatcher(TopLevelRecordMatcher,
                new checks::indentation::StructUnionBodyIndentChecker(PP));
        Matcher.addMatcher(TopLevelEnumMatcher,
                new checks::indentation::EnumBodyIndentChecker(PP));
        Matcher.addMatcher(TopLevelTypedefMatcher,
                new checks::indentation::TypedefIndentChecker());

        // OVERALL checks
        Matcher.addMatcher(FuncDeclMatcher,
                new checks::overall::FuncDefinitionInHeaderChecker());
        Matcher.addMatcher(
                FuncDeclMatcher, new checks::overall::FuncLengthChecker());

        // WARNING checks
        Matcher.addMatcher(GoToMatcher, new checks::warning::GoToChecker());
    }

    void HandleTranslationUnit(ASTContext& Context) override {
        Matcher.matchAST(Context);
    }

    private:
    MatchFinder Matcher;
};

// The FrontEndAction is the main entry point for the clang tooling library
// and allows us to add callbacks for checks via PPCallback and
// ASTConsumer classes.
class NettFrontEndAction : public ASTFrontendAction {

    public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(
            CompilerInstance& CI, StringRef file) override {

        // Here we add any checks which require the preprocessor.
        CI.getPreprocessor().addPPCallbacks(std::unique_ptr<PPCallbacks>(
                new checks::naming::DefineNameChecker(CI.getSourceManager())));

        CI.getPreprocessor().addPPCallbacks(std::unique_ptr<PPCallbacks>(
                new checks::overall::KeywordRedefChecker(
                        CI.getSourceManager(), CI.getLangOpts())));

        return std::unique_ptr<ASTConsumer>(
                new NettASTConsumer(CI.getPreprocessor()));
    }
};

// Appends a set of default flags to the command line arguments passed
// to the program. These flags are necessary to allow correct functionality
// and make invocation simpler.
std::vector<std::string> AppendDefaultFlagsToArgv(int Argc, const char** Argv) {

    std::vector<std::string> NewArgv;
    std::vector<std::string> DefaultFlags{"--extra-arg=-fparse-all-comments",
            "--extra-arg=-std=gnu99", "--extra-arg=-trigraphs", "--"};

    // We take the command line arguments as is
    for (int i = 0; i < Argc; i++) {
        NewArgv.push_back(Argv[i]);
    }

    // Then we add our flags
    for (auto Flag : DefaultFlags) {
        NewArgv.push_back(Flag);
    }

    return NewArgv;
}

// Prints the licensing information for the program.
void PrintLicense(void) {
    llvm::outs() << "nett  Copyright (C) 2020  Matthys Grobbelaar\n\n";
    llvm::outs() << "Licensed under the GNU General Public License v3.0.\n";
    llvm::outs() << "License details can be found at "
                    "https://github.com/dgrobb/nett\n";
}

// NETT ------------------------------------------------------------------
int main(int Argc, const char** Argv) {

    // We need to add default flags to the command line arguments before
    // they are parsed.
    std::vector<std::string> NewArgvStrings =
            AppendDefaultFlagsToArgv(Argc, Argv);
    std::vector<const char*> NewArgv;

    for (const auto& Arg : NewArgvStrings) {
        NewArgv.push_back(Arg.c_str());
    }

    // Once we've modified the arguments, we can then parse them
    int NewArgc = NewArgv.size();
    CommonOptionsParser OptionsParser(
            NewArgc, NewArgv.data(), NettOptionCategory, cl::ZeroOrMore);

    // If they want the license, show it and exit
    if (ShowLicense) {
        PrintLicense();
        return EXIT_SUCCESS;
    }

    // Check if we received some input files.
    // Also check if files are accessible
    auto FileList = OptionsParser.getSourcePathList();

    if (FileList.empty()) {
        llvm::errs() << "Error: No input files specified\n";
        return EXIT_FAILURE;
    }

    for (auto FilePath : FileList) {
        if (!input::FileCanBeAccessed(FilePath)) {
            llvm::errs() << "Error: Unable to access file "
                         << colors::Colorize(FilePath, colors::COLOR_BOLD_WHITE)
                         << "\n";
            return EXIT_FAILURE;
        }
    }

    // We have some files, so we can set up a new clang tool to do the checks.
    ClangTool Tool(OptionsParser.getCompilations(), FileList);
    Tool.setDiagnosticConsumer(new WarningDiagConsumer);
    if (GenerateFiles) {
        GlobalViolationManager.SetOutputToFile();
    }

    // Once the tool has been set up, we need to convert all tabs into spaces
    // within the file contents. In order to avoid needing to create a new file
    // on disk, this can be done by providing a virtual content mapping of the
    // file to the tool. We also need to keep track of the file content
    // since the virtual map uses StringRefs which require the strings to have
    // a long lifetime and not doing this causes memory corruption and annoying
    // bugs.
    std::vector<std::string> FileContentHolder;
    int FileIndex = 0;

    for (const auto& FilePath : OptionsParser.getSourcePathList()) {
        llvm::SmallString<128> AbsPath(FilePath);
        Tool.getFiles().makeAbsolutePath(AbsPath);
        std::string Content = input::GetSanitizedFileContent(AbsPath.c_str());
        FileContentHolder.push_back(Content);
        Tool.mapVirtualFile(
                FilePath, llvm::StringRef(FileContentHolder.at(FileIndex)));
        FileIndex++;
    }

    // Now that the tool is all set up, we can run the checks.
    Tool.run(newFrontendActionFactory<NettFrontEndAction>().get());

    // Some of the checks (e.g. function comments) can only be done once
    // everything has been processed. We do those checks here.
    GlobalFunctionCommentManager.GenerateCommentViolations();

    // Once the checks are done, we output the violations.
    // We need to get the full paths to each of the files due to the way
    // the violations are stored.
    std::vector<std::string> FilePaths;
    for (auto FilePath : FileList) {
        llvm::SmallString<128> AbsPath(FilePath);
        Tool.getFiles().makeAbsolutePath(AbsPath);
        FilePaths.push_back(AbsPath.c_str());
    }

    if (GenerateFiles) {
        output::OutputViolationsToFiles(FilePaths);
    } else {
        output::OutputViolationsToTerminal(FilePaths);
    }

    return EXIT_SUCCESS;
}
