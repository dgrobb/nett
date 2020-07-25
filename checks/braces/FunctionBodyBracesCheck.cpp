/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FunctionBodyBracesCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "../whitespace/WhitespaceCheck.hpp"
#include "BracesCheck.hpp"

#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace braces {

static bool HasBraces(clang::Stmt* Stmt);
static void CheckForMissingBraces(
        clang::Stmt* Stmt, clang::ASTContext* Context);

class FunctionASTVisitor
    : public clang::RecursiveASTVisitor<FunctionASTVisitor> {
    using Base = clang::RecursiveASTVisitor<FunctionASTVisitor>;

    public:
    explicit FunctionASTVisitor(clang::ASTContext* Context) : Context(Context) {
    }

    bool VisitIfStmt(clang::IfStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto File = SM.getFilename(Stmt->getBeginLoc());

        CheckForMissingBraces(Stmt->getThen(), Context);
        if (HasBraces(Stmt->getThen())) {
            auto* Body = llvm::dyn_cast<clang::CompoundStmt>(Stmt->getThen());
            auto OpenBraceLoc = Body->getLBracLoc();
            auto ConditionEndLoc = whitespace::GetPreviousNonWhitespaceLoc(
                    OpenBraceLoc.getLocWithOffset(-1), SM);

            if (SM.getExpansionLineNumber(ConditionEndLoc) !=
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Opening braces should look like: if (cond) {";

                auto LineNo = SM.getExpansionLineNumber(OpenBraceLoc);
                GlobalViolationManager.AddViolation(
                        new BracesViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }

        if (Stmt->hasElseStorage()) {
            auto ElseLoc = Stmt->getElseLoc();
            if (HasBraces(Stmt->getThen())) {
                // We need to check the right curly brace before the else
                auto* ThenBody =
                        llvm::dyn_cast<clang::CompoundStmt>(Stmt->getThen());
                auto RBraceLoc = ThenBody->getRBracLoc();

                if (SM.getExpansionLineNumber(RBraceLoc) !=
                        SM.getExpansionLineNumber(ElseLoc)) {
                    std::stringstream ErrMsg;
                    ErrMsg << "Closing braces should look like: } else";

                    auto LineNo = SM.getExpansionLineNumber(RBraceLoc);
                    GlobalViolationManager.AddViolation(new BracesViolation(
                            File.str(), LineNo, ErrMsg.str()));
                }
            }
            if (!llvm::isa<clang::IfStmt>(Stmt->getElse())) {
                // We only want the 'else' clause, not 'else if'
                CheckForMissingBraces(Stmt->getElse(), Context);

                if (HasBraces(Stmt->getElse())) {
                    // We need to check the left curly brace after the else
                    auto* ElseBody = llvm::dyn_cast<clang::CompoundStmt>(
                            Stmt->getElse());
                    auto LBraceLoc = ElseBody->getLBracLoc();

                    if (SM.getExpansionLineNumber(ElseLoc) !=
                            SM.getExpansionLineNumber(LBraceLoc)) {
                        std::stringstream ErrMsg;
                        ErrMsg << "Opening braces should look like: else {";

                        auto LineNo = SM.getExpansionLineNumber(LBraceLoc);
                        GlobalViolationManager.AddViolation(new BracesViolation(
                                File.str(), LineNo, ErrMsg.str()));
                    }
                }
            }
        }

        return true;
    }

    bool VisitForStmt(clang::ForStmt* Stmt) {
        CheckForMissingBraces(Stmt->getBody(), Context);
        if (HasBraces(Stmt->getBody())) {
            auto& SM = Context->getSourceManager();
            auto LangOpts = Context->getLangOpts();
            auto File = SM.getFilename(Stmt->getBeginLoc());
            auto* Body = llvm::dyn_cast<clang::CompoundStmt>(Stmt->getBody());
            auto OpenBraceLoc = Body->getLBracLoc();

            auto ConditionEndLoc = whitespace::GetPreviousNonWhitespaceLoc(
                    OpenBraceLoc.getLocWithOffset(-1), SM);

            if (SM.getExpansionLineNumber(ConditionEndLoc) !=
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Opening braces should look like: for (...;...;...) "
                          "{";

                auto LineNo = SM.getExpansionLineNumber(OpenBraceLoc);
                GlobalViolationManager.AddViolation(
                        new BracesViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }
        return true;
    }

    bool VisitDoStmt(clang::DoStmt* Stmt) {
        CheckForMissingBraces(Stmt->getBody(), Context);
        if (HasBraces(Stmt->getBody())) {
            auto& SM = Context->getSourceManager();
            auto LangOpts = Context->getLangOpts();
            auto File = SM.getFilename(Stmt->getBeginLoc());
            auto* Body = llvm::dyn_cast<clang::CompoundStmt>(Stmt->getBody());

            // Check the position of the open curly brace after the do
            auto DoLoc = Stmt->getDoLoc();
            auto OpenBraceLoc = Body->getLBracLoc();
            if (SM.getExpansionLineNumber(DoLoc) !=
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Opening braces should look like: do {";

                auto LineNo = SM.getExpansionLineNumber(OpenBraceLoc);
                GlobalViolationManager.AddViolation(
                        new BracesViolation(File.str(), LineNo, ErrMsg.str()));
            }

            // Check the position of the closing curly brace before the while
            auto WhileLoc = Stmt->getWhileLoc();
            auto CloseBraceLoc = Body->getRBracLoc();
            if (SM.getExpansionLineNumber(CloseBraceLoc) !=
                    SM.getExpansionLineNumber(WhileLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Closing braces should look like: } while (cond)";

                auto LineNo = SM.getExpansionLineNumber(CloseBraceLoc);
                GlobalViolationManager.AddViolation(
                        new BracesViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }
        return true;
    }

    bool VisitWhileStmt(clang::WhileStmt* Stmt) {
        CheckForMissingBraces(Stmt->getBody(), Context);
        if (HasBraces(Stmt->getBody())) {
            auto& SM = Context->getSourceManager();
            auto LangOpts = Context->getLangOpts();
            auto File = SM.getFilename(Stmt->getBeginLoc());
            auto* Body = llvm::dyn_cast<clang::CompoundStmt>(Stmt->getBody());
            auto OpenBraceLoc = Body->getLBracLoc();
            auto ConditionEndLoc = whitespace::GetPreviousNonWhitespaceLoc(
                    OpenBraceLoc.getLocWithOffset(-1), SM);

            if (SM.getExpansionLineNumber(ConditionEndLoc) !=
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Opening braces should look like: while (cond) {";

                auto LineNo = SM.getExpansionLineNumber(OpenBraceLoc);
                GlobalViolationManager.AddViolation(
                        new BracesViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }
        return true;
    }

    bool VisitSwitchStmt(clang::SwitchStmt* Stmt) {
        CheckForMissingBraces(Stmt->getBody(), Context);
        if (HasBraces(Stmt->getBody())) {
            auto& SM = Context->getSourceManager();
            auto LangOpts = Context->getLangOpts();
            auto File = SM.getFilename(Stmt->getBeginLoc());
            auto* Body = llvm::dyn_cast<clang::CompoundStmt>(Stmt->getBody());
            auto OpenBraceLoc = Body->getLBracLoc();
            auto ConditionEndLoc = whitespace::GetPreviousNonWhitespaceLoc(
                    OpenBraceLoc.getLocWithOffset(-1), SM);

            if (SM.getExpansionLineNumber(ConditionEndLoc) !=
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Opening braces should look like: switch (cond) {";

                auto LineNo = SM.getExpansionLineNumber(OpenBraceLoc);
                GlobalViolationManager.AddViolation(
                        new BracesViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }
        return true;
    }

    bool VisitCaseStmt(clang::CaseStmt* Stmt) {

        if (HasBraces(Stmt->getSubStmt())) {
            auto* Body =
                    llvm::dyn_cast<clang::CompoundStmt>(Stmt->getSubStmt());
            auto OpenBraceLoc = Body->getLBracLoc();
            auto CaseEndLoc = Stmt->getCaseLoc();
            auto& SM = Context->getSourceManager();

            if (SM.getExpansionLineNumber(CaseEndLoc) !=
                    SM.getExpansionLineNumber(OpenBraceLoc)) {

                std::stringstream ErrMsg;
                ErrMsg << "Opening braces should look like: case X: {";

                auto File = SM.getFilename(Stmt->getBeginLoc());
                auto LineNo = SM.getExpansionLineNumber(OpenBraceLoc);
                GlobalViolationManager.AddViolation(
                        new BracesViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }

        return true;
    }

    bool VisitCompoundStmt(clang::CompoundStmt* Stmt) {
        CheckBraceIsAtEndOfLine(Stmt->getLBracLoc(), Context);
        return true;
    }

    clang::ASTContext* Context;
};

void FunctionBodyBracesChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {

        auto& SM = *Result.SourceManager;
        auto File = SM.getFilename(Node->getLocation());

        if (!SM.isWrittenInMainFile(Node->getLocation())) {
            return;
        }

        if (!Node->isThisDeclarationADefinition()) {
            return;
        }

        auto* FuncBody = llvm::dyn_cast<clang::CompoundStmt>(Node->getBody());
        auto BodyLBraceLoc = FuncBody->getLBracLoc();
        auto ParamEndParenLoc = whitespace::GetPreviousNonWhitespaceLoc(
                BodyLBraceLoc.getLocWithOffset(-1), SM);

        auto ParamEndLine = SM.getExpansionLineNumber(ParamEndParenLoc);
        auto BodyLBraceLine = SM.getExpansionLineNumber(BodyLBraceLoc);

        if (BodyLBraceLine - ParamEndLine > 1) {
            std::stringstream ErrMsg;
            ErrMsg << "The opening brace of a function should be at most one "
                      "line away from its parameters.";

            auto LineNo = SM.getExpansionLineNumber(BodyLBraceLoc);
            GlobalViolationManager.AddViolation(
                    new BracesViolation(File.str(), LineNo, ErrMsg.str()));
        }
        if (BodyLBraceLine != ParamEndLine &&
                SM.getSpellingColumnNumber(BodyLBraceLoc) != 1) {
            std::stringstream ErrMsg;
            ErrMsg << "The opening brace of a function should be left-aligned "
                      "if not on the same line as its parameters.";

            auto LineNo = SM.getExpansionLineNumber(BodyLBraceLoc);
            GlobalViolationManager.AddViolation(
                    new BracesViolation(File.str(), LineNo, ErrMsg.str()));
        }

        FunctionASTVisitor Visitor(Result.Context);
        Visitor.TraverseDecl(const_cast<clang::FunctionDecl*>(Node));
    }
}

// Checks if the given statement is a compound statement.
// If it is, returns true, else returns false.
static bool HasBraces(clang::Stmt* Stmt) {
    return llvm::isa<clang::CompoundStmt>(Stmt);
}

// Checks if the given statement is a compound statement and generates
// a braces violation if it is not such a statement.
static void CheckForMissingBraces(
        clang::Stmt* Stmt, clang::ASTContext* Context) {

    if (!HasBraces(Stmt)) {
        auto& SM = Context->getSourceManager();
        auto File = SM.getFilename(Stmt->getBeginLoc());
        auto LineNo = SM.getExpansionLineNumber(Stmt->getBeginLoc());

        std::stringstream ErrMsg;
        ErrMsg << "Braces are required, even for single line blocks.";

        GlobalViolationManager.AddViolation(
                new BracesViolation(File.str(), LineNo, ErrMsg.str()));
    }
}

}  // namespace braces
}  // namespace checks
}  // namespace nett