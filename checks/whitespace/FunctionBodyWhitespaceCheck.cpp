/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FunctionBodyWhitespaceCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "WhitespaceCheck.hpp"

#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace whitespace {

class FunctionASTVisitor
    : public clang::RecursiveASTVisitor<FunctionASTVisitor> {
    using Base = clang::RecursiveASTVisitor<FunctionASTVisitor>;

    public:
    explicit FunctionASTVisitor(
            clang::ASTContext* Context, clang::Preprocessor& PP)
        : Context(Context), PP(PP) {
    }

    bool VisitIfStmt(clang::IfStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto File = SM.getFilename(Stmt->getBeginLoc());
        auto IfLoc = Stmt->getIfLoc();
        auto CondParenLoc = utils::FindCharLocation(IfLoc, '(', SM, Context);

        // First we check the whitespace between the if and the
        // condition. If statements should look like if (cond...
        auto IfLineNo = SM.getExpansionLineNumber(IfLoc);

        if (IfLineNo != SM.getExpansionLineNumber(CondParenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "If statements should look like: if (cond...";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File, IfLineNo, ErrMsg.str()));
        } else {
            CheckLocationWhitespace(
                    IfLoc.getLocWithOffset(1), CondParenLoc, 1, SM, LangOpts);
        }

        // Next, we check the whitespace within the condition
        auto LParenLoc = CondParenLoc;
        auto CondRange = Stmt->getCond()->getSourceRange();
        auto RParenLoc = utils::FindCharLocation(
                SM.getExpansionRange(CondRange).getEnd().getLocWithOffset(1),
                ')', SM, Context);

        // Stmt->getCond()->getEndLoc().getLocWithOffset(1)
        CheckParenWhitespace(LParenLoc, RParenLoc, SM, LangOpts);

        // Next, we check the whitespace between the condition and the
        // opening brace -> ...cond) {
        if (llvm::isa<clang::CompoundStmt>(Stmt->getThen())) {
            CheckLocationWhitespace(
                    RParenLoc, Stmt->getThen()->getBeginLoc(), 1, SM, LangOpts);
        }

        // Next, we check the whitespace between the closing brace and
        // the else statement
        if (llvm::isa<clang::CompoundStmt>(Stmt->getThen()) &&
                Stmt->hasElseStorage()) {
            CheckLocationWhitespace(Stmt->getThen()->getEndLoc(),
                    Stmt->getElseLoc(), 1, SM, LangOpts);
        }

        // Then we check the whitespace between the else and the if
        if (Stmt->hasElseStorage() &&
                llvm::isa<clang::IfStmt>(Stmt->getElse())) {
            auto ElseLineNo = SM.getExpansionLineNumber(Stmt->getElseLoc());
            auto IfLineNo =
                    SM.getExpansionLineNumber(Stmt->getElse()->getBeginLoc());

            if (ElseLineNo != IfLineNo) {
                std::stringstream ErrMsg;
                ErrMsg << "else-if statements should look like: } else if "
                          "(cond...";
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File, ElseLineNo, ErrMsg.str()));
            } else {
                CheckLocationWhitespace(Stmt->getElseLoc().getLocWithOffset(3),
                        Stmt->getElse()->getBeginLoc(), 1, SM, LangOpts);
            }
        }

        // Finally, we check the space between the else and the open curly brace
        if (Stmt->hasElseStorage() &&
                !llvm::isa<clang::IfStmt>(Stmt->getElse())) {
            if (llvm::isa<clang::CompoundStmt>(Stmt->getElse())) {
                auto ElseLineNo = SM.getExpansionLineNumber(Stmt->getElseLoc());
                auto BraceLineNo = SM.getExpansionLineNumber(
                        Stmt->getElse()->getBeginLoc());

                if (BraceLineNo == ElseLineNo) {
                    CheckLocationWhitespace(
                            Stmt->getElseLoc().getLocWithOffset(3),
                            Stmt->getElse()->getBeginLoc(), 1, SM, LangOpts);
                }
            }
        }

        return true;
    }

    bool VisitForStmt(clang::ForStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto File = SM.getFilename(Stmt->getBeginLoc());

        // First, we check the space between the 'for'
        // and the condition parenthesis
        auto ForLoc = Stmt->getForLoc().getLocWithOffset(2);
        auto LParenLoc = utils::FindCharLocation(ForLoc, '(', SM, Context);
        if (SM.getExpansionLineNumber(ForLoc) !=
                SM.getExpansionLineNumber(LParenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "For loops should look like: for (...";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File, SM.getExpansionLineNumber(ForLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(ForLoc, LParenLoc, 1, SM, LangOpts);
        }

        // Then we check the condition's whitespace
        auto RParenLoc = Stmt->getBody()->getBeginLoc();
        while (*SM.getCharacterData(RParenLoc) != ')') {
            RParenLoc = RParenLoc.getLocWithOffset(-1);
        }
        CheckParenWhitespace(LParenLoc, RParenLoc, SM, LangOpts);

        // Next, we check the space between the end
        // of the condition and the opening curly brace
        if (llvm::isa<clang::CompoundStmt>(Stmt->getBody())) {
            if (SM.getExpansionLineNumber(RParenLoc) ==
                    SM.getExpansionLineNumber(Stmt->getBody()->getBeginLoc())) {
                CheckLocationWhitespace(RParenLoc,
                        Stmt->getBody()->getBeginLoc(), 1, SM, LangOpts);
            }
        }

        // Finally, we check the spacing around the semicolons
        // within the condition
        auto InitSemiLoc = utils::FindCharLocation(LParenLoc, ';', SM, Context);
        auto IncSemiLoc = utils::FindCharLocation(
                InitSemiLoc.getLocWithOffset(1), ';', SM, Context);
        auto InitEndLoc = GetPreviousNonWhitespaceLoc(
                InitSemiLoc.getLocWithOffset(-1), SM);
        auto CondStartLoc =
                GetNextNonWhitespaceLoc(InitSemiLoc.getLocWithOffset(1), SM);
        auto CondEndLoc = GetPreviousNonWhitespaceLoc(
                IncSemiLoc.getLocWithOffset(-1), SM);

        if (Stmt->getInit()) {
            // The semicolon should be on the same line as the end of the
            // initialiser.
            auto InitEndLineNo = SM.getExpansionLineNumber(InitEndLoc);
            auto SemiLineNo = SM.getExpansionLineNumber(InitSemiLoc);

            if (InitEndLineNo != SemiLineNo) {
                std::stringstream ErrMsg;
                ErrMsg << "For loop initialisers should look like: for (init; "
                          "...";
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File, InitEndLineNo, ErrMsg.str()));
            } else {
                CheckLocationWhitespace(
                        InitEndLoc, InitSemiLoc, 0, SM, LangOpts);
            }
        }

        if (Stmt->getCond()) {
            // There should be a single space between the previous semicolon if
            // they are on the same line. Also, the next semicolon should be
            // on the same line as the end of the condition.
            auto InitSemiLineNo = SM.getExpansionLineNumber(InitSemiLoc);
            auto CondStartLineNo = SM.getExpansionLineNumber(CondStartLoc);
            auto CondEndLineNo = SM.getExpansionLineNumber(CondEndLoc);
            auto IncSemiLineNo = SM.getExpansionLineNumber(IncSemiLoc);

            if (InitSemiLineNo == CondStartLineNo) {
                CheckLocationWhitespace(
                        InitSemiLoc, CondStartLoc, 1, SM, LangOpts);
            }

            if (CondEndLineNo != IncSemiLineNo) {
                std::stringstream ErrMsg;
                ErrMsg << "For loop conditions should look like: for (...; "
                          "cond; ...";
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File, CondEndLineNo, ErrMsg.str()));
            } else {
                CheckLocationWhitespace(
                        CondEndLoc, IncSemiLoc, 0, SM, LangOpts);
            }
        } else {
            // If there is no condition, there should be no space between
            // the two semicolons
            auto InitSemiLineNo = SM.getExpansionLineNumber(InitSemiLoc);
            auto IncSemiLineNo = SM.getExpansionLineNumber(IncSemiLoc);

            if (InitSemiLineNo != IncSemiLineNo) {
                std::stringstream ErrMsg;
                ErrMsg << "Missing for loop conditions should look like: for "
                          "(...;; ...";
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File, InitSemiLineNo, ErrMsg.str()));
            } else {
                CheckLocationWhitespace(
                        InitSemiLoc, IncSemiLoc, 0, SM, LangOpts);
            }
        }

        return true;
    }

    bool VisitDoStmt(clang::DoStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto File = SM.getFilename(Stmt->getBeginLoc());

        if (llvm::isa<clang::CompoundStmt>(Stmt->getBody())) {

            // First we check the spacing between the 'do' and the open curly
            // brace
            auto DoLoc = Stmt->getDoLoc().getLocWithOffset(1);
            auto OpenBraceLoc = Stmt->getBody()->getBeginLoc();

            if (SM.getExpansionLineNumber(DoLoc) ==
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                CheckLocationWhitespace(DoLoc, OpenBraceLoc, 1, SM, LangOpts);
            }

            // Then we check the spacing between the closing curly brace and the
            // 'while'
            auto CloseBraceLoc = Stmt->getBody()->getEndLoc();
            auto WhileLoc = Stmt->getWhileLoc();

            if (SM.getExpansionLineNumber(CloseBraceLoc) ==
                    SM.getExpansionLineNumber(WhileLoc)) {
                CheckLocationWhitespace(
                        CloseBraceLoc, WhileLoc, 1, SM, LangOpts);
            }
        }

        // Then we check the spacing between the 'while' and the condition.
        // Both must be on the same line, and must be separated by a single
        // space
        auto WhileEndLoc = Stmt->getWhileLoc().getLocWithOffset(4);
        auto CondStartLoc =
                utils::FindCharLocation(WhileEndLoc, '(', SM, Context);
        auto CondLineNo = SM.getExpansionLineNumber(CondStartLoc);

        if (SM.getExpansionLineNumber(WhileEndLoc) != CondLineNo) {
            std::stringstream ErrMsg;
            ErrMsg << "Do-While loop conditions should look like: while (...";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File, CondLineNo, ErrMsg.str()));
        } else {
            CheckLocationWhitespace(WhileEndLoc, CondStartLoc, 1, SM, LangOpts);
        }

        // Next, we check the spacing within the condition
        auto CondRange = Stmt->getCond()->getSourceRange();
        auto CondEndLoc = utils::FindCharLocation(
                SM.getExpansionRange(CondRange).getEnd().getLocWithOffset(1),
                ')', SM, Context);
        CheckParenWhitespace(CondStartLoc, CondEndLoc, SM, LangOpts);

        // Finally, we check the space between the end of the condition
        // and the semicolon
        auto SemiLoc = utils::FindCharLocation(CondEndLoc, ';', SM, Context);
        auto SemiLineNo = SM.getExpansionLineNumber(SemiLoc);

        if (SM.getExpansionLineNumber(CondEndLoc) != SemiLineNo) {
            std::stringstream ErrMsg;
            ErrMsg << "Do-While loop conditions should look like: ...);";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File, CondLineNo, ErrMsg.str()));
        } else {
            CheckLocationWhitespace(CondEndLoc, SemiLoc, 0, SM, LangOpts);
        }

        return true;
    }

    bool VisitWhileStmt(clang::WhileStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto File = SM.getFilename(Stmt->getBeginLoc());

        // First, we check the space between the 'while' and the condition.
        auto WhileLoc = Stmt->getWhileLoc().getLocWithOffset(4);
        auto CondStartLoc = utils::FindCharLocation(WhileLoc, '(', SM, Context);

        if (SM.getExpansionLineNumber(WhileLoc) !=
                SM.getExpansionLineNumber(CondStartLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "While loops should look like: while (...";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File, SM.getExpansionLineNumber(WhileLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(WhileLoc, CondStartLoc, 1, SM, LangOpts);
        }

        // Then we check the condition spacing
        auto CondRange = Stmt->getCond()->getSourceRange();
        auto CondEndLoc = utils::FindCharLocation(
                SM.getExpansionRange(CondRange).getEnd().getLocWithOffset(1),
                ')', SM, Context);
        CheckParenWhitespace(CondStartLoc, CondEndLoc, SM, LangOpts);

        // Finally we check space between the condition and the open curly brace
        if (llvm::isa<clang::CompoundStmt>(Stmt->getBody())) {
            auto OpenBraceLoc = Stmt->getBody()->getBeginLoc();

            if (SM.getExpansionLineNumber(CondEndLoc) ==
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                CheckLocationWhitespace(
                        CondEndLoc, OpenBraceLoc, 1, SM, LangOpts);
            }
        }

        return true;
    }

    bool VisitSwitchStmt(clang::SwitchStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto File = SM.getFilename(Stmt->getBeginLoc());

        // First, we check the space between the 'switch' and the condition.
        auto SwitchLoc = Stmt->getSwitchLoc().getLocWithOffset(5);
        auto CondStartLoc =
                utils::FindCharLocation(SwitchLoc, '(', SM, Context);

        if (SM.getExpansionLineNumber(SwitchLoc) !=
                SM.getExpansionLineNumber(CondStartLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Switch statements should look like: switch (...";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File, SM.getExpansionLineNumber(SwitchLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(SwitchLoc, CondStartLoc, 1, SM, LangOpts);
        }

        // Then we check the condition spacing
        auto CondRange = Stmt->getCond()->getSourceRange();
        auto CondEndLoc = utils::FindCharLocation(
                SM.getExpansionRange(CondRange).getEnd().getLocWithOffset(1),
                ')', SM, Context);
        CheckParenWhitespace(CondStartLoc, CondEndLoc, SM, LangOpts);

        // Finally we check space between the condition and the open curly brace
        if (llvm::isa<clang::CompoundStmt>(Stmt->getBody())) {
            auto OpenBraceLoc = Stmt->getBody()->getBeginLoc();

            if (SM.getExpansionLineNumber(CondEndLoc) ==
                    SM.getExpansionLineNumber(OpenBraceLoc)) {
                CheckLocationWhitespace(
                        CondEndLoc, OpenBraceLoc, 1, SM, LangOpts);
            }
        }

        return true;
    }

    bool VisitCaseStmt(clang::CaseStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto File = SM.getFilename(Stmt->getBeginLoc());

        // First, we check that the case and its associated expression are
        // on the same line and are one space apart
        auto CaseLoc = Stmt->getCaseLoc().getLocWithOffset(3);
        auto CondStartLoc =
                GetNextNonWhitespaceLoc(CaseLoc.getLocWithOffset(1), SM);

        if (SM.getExpansionLineNumber(CaseLoc) !=
                SM.getExpansionLineNumber(CondStartLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Case statements should look like: case X:";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File, SM.getExpansionLineNumber(CaseLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(CaseLoc, CondStartLoc, 1, SM, LangOpts);
        }

        // Then we check that the expression and the colon are on the
        // same line and have no spaces between them.
        auto ColonLoc = Stmt->getColonLoc();
        auto CondEndLoc =
                GetPreviousNonWhitespaceLoc(ColonLoc.getLocWithOffset(-1), SM);

        if (SM.getExpansionLineNumber(CondEndLoc) !=
                SM.getExpansionLineNumber(ColonLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Case expressions should look like: case X:";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File, SM.getExpansionLineNumber(CondEndLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(CondEndLoc, ColonLoc, 0, SM, LangOpts);
        }

        // Finally, if the case statement is a compound statement, we
        // check that the colon and open brace are a single space apart
        if (llvm::isa<clang::CompoundStmt>(Stmt->getSubStmt())) {
            auto BraceLoc = Stmt->getSubStmt()->getBeginLoc();

            if (SM.getExpansionLineNumber(BraceLoc) ==
                    SM.getExpansionLineNumber(ColonLoc)) {
                CheckLocationWhitespace(ColonLoc, BraceLoc, 1, SM, LangOpts);
            }
        }

        return true;
    }

    clang::ASTContext* Context;
    clang::Preprocessor& PP;
};

void FunctionBodyWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {

        auto& SM = *Result.SourceManager;
        auto File = SM.getFilename(Node->getBeginLoc());
        auto LangOpts = Result.Context->getLangOpts();

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        if (Node->isThisDeclarationADefinition()) {
            FunctionASTVisitor Visitor(Result.Context, PP);
            Visitor.TraverseDecl(const_cast<clang::FunctionDecl*>(Node));
        } else {
            // There should be no space between the end of the declaration
            // and the closing semi-colon
            auto SemiLoc = utils::FindCharLocation(
                    Node->getEndLoc(), ';', SM, Result.Context);
            auto DeclEndLoc = GetPreviousNonWhitespaceLoc(
                    SemiLoc.getLocWithOffset(-1), SM);
            auto SemiLineNo = SM.getExpansionLineNumber(SemiLoc);

            if (SemiLineNo != SM.getExpansionLineNumber(DeclEndLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Semicolons should be on the same line as the end of "
                          "a declaration.";
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File, SemiLineNo, ErrMsg.str()));
            } else {
                CheckLocationWhitespace(DeclEndLoc, SemiLoc, 0, SM, LangOpts);
            }
        }

        // We require that the return type of the function is on the
        // same line as its name. For example: int main(...)
        if (SM.getExpansionLineNumber(Node->getLocation()) !=
                SM.getExpansionLineNumber(Node->getBeginLoc())) {
            std::stringstream ErrMsg;
            ErrMsg << "A function's return type should be on the same line as "
                      "its name.";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(File,
                    SM.getExpansionLineNumber(Node->getBeginLoc()),
                    ErrMsg.str()));
        }

        // The opening parameter parenthesis and the function name should
        // have no spaces between them
        auto LParenLoc = utils::FindCharLocation(
                Node->getLocation(), '(', SM, Result.Context);
        auto NameEndLoc =
                GetPreviousNonWhitespaceLoc(LParenLoc.getLocWithOffset(-1), SM);

        if (SM.getExpansionLineNumber(LParenLoc) !=
                SM.getExpansionLineNumber(NameEndLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Function declarations should look like func_name(...";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File, SM.getExpansionLineNumber(LParenLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(NameEndLoc, LParenLoc, 0, SM, LangOpts);
        }

        // The first and last function parameters should have no space between
        // them and the declaration parentheses
        clang::SourceLocation RParenLoc;
        if (Node->getNumParams() == 0) {
            RParenLoc =
                    utils::FindCharLocation(LParenLoc, ')', SM, Result.Context);
        } else {
            auto LastParamLoc =
                    Node->getParamDecl(Node->getNumParams() - 1)->getEndLoc();
            RParenLoc = utils::FindCharLocation(
                    LastParamLoc.getLocWithOffset(1), ')', SM, Result.Context);
        }
        CheckParenWhitespace(LParenLoc, RParenLoc, SM, LangOpts);

        // Finally, we check the spacing around commas in parameters
        CheckSourceRangeCommaWhitespace(LParenLoc, RParenLoc, SM, LangOpts);
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett