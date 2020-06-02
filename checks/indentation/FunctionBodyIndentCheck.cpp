/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FunctionBodyIndentCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "../utils/Typedef.hpp"
#include "../whitespace/WhitespaceCheck.hpp"
#include "EnumBodyIndentCheck.hpp"
#include "IndentCheck.hpp"
#include "StructUnionBodyIndentCheck.hpp"
#include "TypedefIndentCheck.hpp"

#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace indentation {

static void CheckStandardStatementIndentation(clang::Stmt* Stmt,
        clang::ASTContext* Context, unsigned int NestingLevel,
        bool IsInSwitchStatement, unsigned int SwitchNestingLevel);

static bool StatementIsALoopConstruct(clang::Stmt* Stmt);
static bool StatementIsAConditional(clang::Stmt* Stmt);

class FunctionASTVisitor
    : public clang::RecursiveASTVisitor<FunctionASTVisitor> {
    using Base = clang::RecursiveASTVisitor<FunctionASTVisitor>;

    public:
    explicit FunctionASTVisitor(
            clang::ASTContext* Context, clang::Preprocessor& PP)
        : Context(Context), PP(PP) {
    }

    bool VisitIfStmt(clang::IfStmt* Stmt) {

        // If the 'then' or 'else' components of the if statement are not
        // compound statements, we need to check the nested statement
        // indentation as well
        if (!llvm::isa<clang::CompoundStmt>(Stmt->getThen()) &&
                !StatementIsALoopConstruct(Stmt->getThen()) &&
                !StatementIsAConditional(Stmt->getThen())) {

            // To account for macros, we need to use getFileLoc() to get
            // the actual location within the file source instead of the
            // macro location.
            auto TrueLocation = Context->getSourceManager().getFileLoc(
                    Stmt->getThen()->getBeginLoc());

            CheckStatementIndentation(TrueLocation, Context,
                    CurrentNestingLevel + SwitchNestingLevel);

            if (PP.isAtStartOfMacroExpansion(Stmt->getThen()->getBeginLoc())) {
                CheckSourceRangeContinuationIndent(TrueLocation,
                        checks::utils::GetMacroEndLocation(
                                Stmt->getThen(), Context, PP),
                        Context, CurrentNestingLevel, SwitchNestingLevel);
            } else {
                CheckSourceRangeContinuationIndent(
                        Stmt->getThen()->getBeginLoc(),
                        Stmt->getThen()->getEndLoc(), Context,
                        CurrentNestingLevel, SwitchNestingLevel);
            }
        }
        if (Stmt->hasElseStorage()) {
            if (!llvm::isa<clang::CompoundStmt>(Stmt->getElse()) &&
                    !StatementIsALoopConstruct(Stmt->getElse()) &&
                    !StatementIsAConditional(Stmt->getElse())) {

                auto TrueLocation = Context->getSourceManager().getFileLoc(
                        Stmt->getElse()->getBeginLoc());
                CheckStatementIndentation(TrueLocation, Context,
                        CurrentNestingLevel + SwitchNestingLevel);

                if (PP.isAtStartOfMacroExpansion(
                            Stmt->getElse()->getBeginLoc())) {
                    CheckSourceRangeContinuationIndent(TrueLocation,
                            checks::utils::GetMacroEndLocation(
                                    Stmt->getElse(), Context, PP),
                            Context, CurrentNestingLevel, SwitchNestingLevel);
                } else {
                    CheckSourceRangeContinuationIndent(
                            Stmt->getElse()->getBeginLoc(),
                            Stmt->getElse()->getEndLoc(), Context,
                            CurrentNestingLevel, SwitchNestingLevel);
                }
            }
        }

        return true;
    }

    bool VisitWhileStmt(clang::WhileStmt* Stmt) {

        if (!llvm::isa<clang::CompoundStmt>(Stmt->getBody()) &&
                !StatementIsALoopConstruct(Stmt->getBody()) &&
                !StatementIsAConditional(Stmt->getBody())) {
            auto TrueLocation = Context->getSourceManager().getFileLoc(
                    Stmt->getBody()->getBeginLoc());
            CheckStatementIndentation(TrueLocation, Context,
                    CurrentNestingLevel + SwitchNestingLevel);
            if (PP.isAtStartOfMacroExpansion(Stmt->getBody()->getBeginLoc())) {
                CheckSourceRangeContinuationIndent(TrueLocation,
                        checks::utils::GetMacroEndLocation(
                                Stmt->getBody(), Context, PP),
                        Context, CurrentNestingLevel, SwitchNestingLevel);
            } else {
                CheckSourceRangeContinuationIndent(
                        Stmt->getBody()->getBeginLoc(),
                        Stmt->getBody()->getEndLoc(), Context,
                        CurrentNestingLevel, SwitchNestingLevel);
            }
        }

        return true;
    }

    bool VisitForStmt(clang::ForStmt* Stmt) {

        if (!llvm::isa<clang::CompoundStmt>(Stmt->getBody()) &&
                !StatementIsALoopConstruct(Stmt->getBody()) &&
                !StatementIsAConditional(Stmt->getBody())) {

            auto TrueLocation = Context->getSourceManager().getFileLoc(
                    Stmt->getBody()->getBeginLoc());
            CheckStatementIndentation(TrueLocation, Context,
                    CurrentNestingLevel + SwitchNestingLevel);
            if (PP.isAtStartOfMacroExpansion(Stmt->getBody()->getBeginLoc())) {
                CheckSourceRangeContinuationIndent(TrueLocation,
                        checks::utils::GetMacroEndLocation(
                                Stmt->getBody(), Context, PP),
                        Context, CurrentNestingLevel, SwitchNestingLevel);
            } else {
                CheckSourceRangeContinuationIndent(
                        Stmt->getBody()->getBeginLoc(),
                        Stmt->getBody()->getEndLoc(), Context,
                        CurrentNestingLevel, SwitchNestingLevel);
            }
        }

        return true;
    }

    bool VisitDoStmt(clang::DoStmt* Stmt) {

        if (!llvm::isa<clang::CompoundStmt>(Stmt->getBody()) &&
                !StatementIsALoopConstruct(Stmt->getBody()) &&
                !StatementIsAConditional(Stmt->getBody())) {
            auto TrueLocation = Context->getSourceManager().getFileLoc(
                    Stmt->getBody()->getBeginLoc());
            CheckStatementIndentation(TrueLocation, Context,
                    CurrentNestingLevel + SwitchNestingLevel);

            if (PP.isAtStartOfMacroExpansion(Stmt->getBody()->getBeginLoc())) {
                CheckSourceRangeContinuationIndent(TrueLocation,
                        checks::utils::GetMacroEndLocation(
                                Stmt->getBody(), Context, PP),
                        Context, CurrentNestingLevel, SwitchNestingLevel);
            } else {
                CheckSourceRangeContinuationIndent(
                        Stmt->getBody()->getBeginLoc(),
                        Stmt->getBody()->getEndLoc(), Context,
                        CurrentNestingLevel, SwitchNestingLevel);
            }
        }

        return true;
    }

    bool VisitSwitchStmt(clang::SwitchStmt* Stmt) {

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();

        auto LParenLoc = whitespace::GetNextNonWhitespaceLoc(
                Stmt->getSwitchLoc().getLocWithOffset(5), SM);
        
        auto RParenLoc = whitespace::GetPreviousNonWhitespaceLoc(
                Stmt->getBody()->getBeginLoc().getLocWithOffset(-1), SM);

        if (llvm::isa<clang::CompoundStmt>(Stmt->getBody())) {
            CheckStatementIndentation(Stmt->getBeginLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel - 1);
            CheckStatementIndentation(Stmt->getEndLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel - 1);
            CheckSourceRangeContinuationIndent(LParenLoc.getLocWithOffset(1),
                    RParenLoc.getLocWithOffset(-1), Context,
                    CurrentNestingLevel - 1, SwitchNestingLevel);
        } else {
            // We have to reduce the nesting level more in order to compensate
            // for the increase added by the TraverseSwitchStmt method for not
            // having braces.
            CheckStatementIndentation(Stmt->getBeginLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel - 2);
            CheckSourceRangeContinuationIndent(LParenLoc.getLocWithOffset(1),
                    RParenLoc.getLocWithOffset(-1), Context,
                    CurrentNestingLevel - 2, SwitchNestingLevel);
        }

        return true;
    }

    bool VisitVarDecl(clang::VarDecl* Decl) {
        if (!llvm::isa<clang::ParmVarDecl>(Decl)) {
            auto ParentNodes = Context->getParents(*Decl);
            auto GrandparentNodes = Context->getParents(
                    *(ParentNodes[0].get<clang::DeclStmt>()));

            // We only want to check the indentation of variables who are
            // part of a compound statement since this avoids declarations
            // inside of loop initialisers etc. which do not require indentation
            // checks (we use whitespace checks instead)
            if (GrandparentNodes[0].get<clang::CompoundStmt>()) {

                // We don't want to match enums or struct definitions
                if (Decl->getType()->getAs<clang::EnumType>()) {
                    return true;
                }
                if (Decl->getType()->getAs<clang::RecordType>()) {
                    if (Decl->hasInit()) {
                        // If we have a struct initialiser, we check its
                        // continuation
                        CheckSourceRangeContinuationIndent(Decl->getBeginLoc(),
                                Decl->getEndLoc(), Context, CurrentNestingLevel,
                                SwitchNestingLevel);
                    }
                    return true;
                }
                // We don't have a special check for switch statements since
                // variables in a switch must be declared at the top (before any
                // cases) if not using braces
                CheckStatementIndentation(
                        Decl->getBeginLoc(), Context, CurrentNestingLevel);
                CheckSourceRangeContinuationIndent(Decl->getBeginLoc(),
                        Decl->getEndLoc(), Context, CurrentNestingLevel,
                        SwitchNestingLevel);
            }
        }
        return true;
    }

    bool VisitReturnStmt(clang::ReturnStmt* Stmt) {

        CheckStandardStatementIndentation(Stmt, Context, CurrentNestingLevel,
                IsInSwitchStatement, SwitchNestingLevel);

        auto ParentNodes = Context->getParents(*Stmt);
        if (ParentNodes[0].get<clang::CompoundStmt>()) {
            CheckSourceRangeContinuationIndent(Stmt->getBeginLoc(),
                    Stmt->getEndLoc(), Context, CurrentNestingLevel,
                    SwitchNestingLevel);
        }
        return true;
    }

    bool VisitBinaryOperator(clang::BinaryOperator* Op) {
        CheckStandardStatementIndentation(Op, Context, CurrentNestingLevel,
                IsInSwitchStatement, SwitchNestingLevel);

        auto ParentNodes = Context->getParents(*Op);
        if (ParentNodes[0].get<clang::CompoundStmt>()) {
            CheckSourceRangeContinuationIndent(Op->getBeginLoc(),
                    Op->getEndLoc(), Context, CurrentNestingLevel,
                    SwitchNestingLevel);
        }

        return true;
    }

    bool VisitUnaryOperator(clang::UnaryOperator* Op) {
        CheckStandardStatementIndentation(Op, Context, CurrentNestingLevel,
                IsInSwitchStatement, SwitchNestingLevel);

        auto ParentNodes = Context->getParents(*Op);
        if (ParentNodes[0].get<clang::CompoundStmt>()) {
            CheckSourceRangeContinuationIndent(Op->getBeginLoc(),
                    Op->getEndLoc(), Context, CurrentNestingLevel,
                    SwitchNestingLevel);
        }

        return true;
    }

    bool VisitCallExpr(clang::CallExpr* Call) {
        CheckStandardStatementIndentation(Call, Context, CurrentNestingLevel,
                IsInSwitchStatement, SwitchNestingLevel);

        auto ParentNodes = Context->getParents(*Call);
        if (ParentNodes[0].get<clang::CompoundStmt>()) {
            CheckSourceRangeContinuationIndent(Call->getBeginLoc(),
                    Call->getEndLoc(), Context, CurrentNestingLevel,
                    SwitchNestingLevel);
        }

        return true;
    }

    bool VisitBreakStmt(clang::BreakStmt* Stmt) {
        CheckStandardStatementIndentation(Stmt, Context, CurrentNestingLevel,
                IsInSwitchStatement, SwitchNestingLevel);
        return true;
    }

    bool VisitContinueStmt(clang::ContinueStmt* Stmt) {
        CheckStandardStatementIndentation(Stmt, Context, CurrentNestingLevel,
                IsInSwitchStatement, SwitchNestingLevel);
        return true;
    }

    bool VisitCaseStmt(clang::CaseStmt* Stmt) {
        CheckStatementIndentation(Stmt->getBeginLoc(), Context,
                CurrentNestingLevel + SwitchNestingLevel - 1);

        if (llvm::isa<clang::CompoundStmt>(Stmt->getSubStmt())) {
            CheckStatementIndentation(Stmt->getEndLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel - 1);
        }
        return true;
    }

    bool VisitDefaultStmt(clang::DefaultStmt* Stmt) {
        CheckStatementIndentation(Stmt->getBeginLoc(), Context,
                CurrentNestingLevel + SwitchNestingLevel - 1);

        if (llvm::isa<clang::CompoundStmt>(Stmt->getSubStmt())) {
            CheckStatementIndentation(Stmt->getEndLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel - 1);
        }
        return true;
    }

    bool VisitCompoundStmt(clang::CompoundStmt* Stmt) {

        // Here we need to check macro indentation.
        // Since we cannot match a macro directly (as it is
        // replaced by its content), we find it via the
        // children of the compound statement.
        for (auto* Child : Stmt->children()) {
            if (StatementIsALoopConstruct(Child) ||
                    StatementIsAConditional(Child)) {
                continue;
            }
            if (PP.isAtStartOfMacroExpansion(Child->getBeginLoc())) {

                auto TrueLocation = Context->getSourceManager().getFileLoc(
                        Child->getBeginLoc());
                CheckStatementIndentation(TrueLocation, Context,
                        CurrentNestingLevel + SwitchNestingLevel);
                CheckSourceRangeContinuationIndent(TrueLocation,
                        checks::utils::GetMacroEndLocation(Child, Context, PP),
                        Context, CurrentNestingLevel, SwitchNestingLevel);
            }
        }

        return true;
    }

    bool VisitRecordDecl(clang::RecordDecl* Decl) {
        CheckStructUnionBodyIndentation(
                Decl, Context, PP, CurrentNestingLevel, SwitchNestingLevel);
        return true;
    }

    bool VisitEnumDecl(clang::EnumDecl* Decl) {
        CheckEnumBodyIndentation(
                Decl, Context, CurrentNestingLevel, SwitchNestingLevel);
        return true;
    }

    bool VisitTypedefDecl(clang::TypedefDecl* Decl) {
        CheckTypedefIndentation(
                Decl, Context, CurrentNestingLevel, SwitchNestingLevel);
        return true;
    }

    bool TraverseCompoundStmt(clang::CompoundStmt* Node) {

        bool IsCaseStatementBody = false;
        bool IsIfStatementBody = false;

        auto ParentNodes = Context->getParents(*Node);
        if (ParentNodes[0].get<clang::CaseStmt>()) {
            IsCaseStatementBody = true;
        } else if (ParentNodes[0].get<clang::IfStmt>()) {
            IsIfStatementBody = true;
        }

        if (!IsCaseStatementBody && !IsIfStatementBody) {
            ++CurrentNestingLevel;
        }

        Base::TraverseCompoundStmt(Node);

        if (!IsCaseStatementBody && !IsIfStatementBody) {
            --CurrentNestingLevel;
        }

        return true;
    }

    bool TraverseSwitchStmt(clang::SwitchStmt* Node) {
        bool SwitchHasNoBraces =
                !llvm::isa<clang::CompoundStmt>(Node->getBody());

        ++SwitchNestingLevel;
        IsInSwitchStatement = true;

        if (SwitchHasNoBraces) {
            CurrentNestingLevel++;
        }

        Base::TraverseSwitchStmt(Node);

        --SwitchNestingLevel;
        if (SwitchNestingLevel == 0) {
            IsInSwitchStatement = false;
        }
        if (SwitchHasNoBraces) {
            CurrentNestingLevel--;
        }

        return true;
    }

    bool TraverseForStmt(clang::ForStmt* Node) {

        CheckStatementIndentation(Node->getBeginLoc(), Context,
                CurrentNestingLevel + SwitchNestingLevel);

        bool ForHasNoBraces = !llvm::isa<clang::CompoundStmt>(Node->getBody());

        if (ForHasNoBraces) {
            CurrentNestingLevel++;
        }

        Base::TraverseForStmt(Node);

        if (ForHasNoBraces) {
            CurrentNestingLevel--;
        } else {
            // Check the right curly brace's location
            auto* CS = llvm::dyn_cast<clang::CompoundStmt>(Node->getBody());
            CheckStatementIndentation(CS->getRBracLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel);
        }

        // Check the condition's continuation indent
        CheckSourceRangeContinuationIndent(
                Node->getLParenLoc().getLocWithOffset(1),
                Node->getRParenLoc().getLocWithOffset(-1), Context,
                CurrentNestingLevel + SwitchNestingLevel, SwitchNestingLevel);

        return true;
    }

    bool TraverseDoStmt(clang::DoStmt* Node) {
        CheckStatementIndentation(Node->getBeginLoc(), Context,
                CurrentNestingLevel + SwitchNestingLevel);

        bool DoHasNoBraces = !llvm::isa<clang::CompoundStmt>(Node->getBody());

        if (DoHasNoBraces) {
            CurrentNestingLevel++;
        }

        Base::TraverseDoStmt(Node);

        if (DoHasNoBraces) {
            CurrentNestingLevel--;
            // If we don't have a compound statement, we need to check the
            // indentation of the 'while' instead of the right curly brace
            CheckStatementIndentation(Node->getWhileLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel);
        } else {
            // Check the right curly brace's location
            auto* CS = llvm::dyn_cast<clang::CompoundStmt>(Node->getBody());
            CheckStatementIndentation(CS->getRBracLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel);
        }

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();

        // Finally, we check the condition's continuation indent
        auto LParenLoc = whitespace::GetNextNonWhitespaceLoc(
                Node->getWhileLoc().getLocWithOffset(5), SM);
        
        auto RParenLoc = Node->getRParenLoc();

        CheckSourceRangeContinuationIndent(LParenLoc.getLocWithOffset(1),
                RParenLoc.getLocWithOffset(-1), Context,
                CurrentNestingLevel + SwitchNestingLevel, SwitchNestingLevel);

        return true;
    }

    bool TraverseWhileStmt(clang::WhileStmt* Node) {

        CheckStatementIndentation(Node->getBeginLoc(), Context,
                CurrentNestingLevel + SwitchNestingLevel);

        bool WhileHasNoBraces =
                !llvm::isa<clang::CompoundStmt>(Node->getBody());

        if (WhileHasNoBraces) {
            CurrentNestingLevel++;
        }

        Base::TraverseWhileStmt(Node);

        if (WhileHasNoBraces) {
            CurrentNestingLevel--;
        } else {
            // If we do have a compound statement, we also check the right curly
            // brace
            auto* CS = llvm::dyn_cast<clang::CompoundStmt>(Node->getBody());
            CheckStatementIndentation(CS->getRBracLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel);
        }

        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();

        // Finally, we check the condition's continuation indent
        auto LParenLoc = whitespace::GetNextNonWhitespaceLoc(
                Node->getWhileLoc().getLocWithOffset(5), SM);
        
        auto RParenLoc = whitespace::GetPreviousNonWhitespaceLoc(
                Node->getBody()->getBeginLoc().getLocWithOffset(-1), SM);

        CheckSourceRangeContinuationIndent(LParenLoc.getLocWithOffset(1),
                RParenLoc.getLocWithOffset(-1), Context,
                CurrentNestingLevel + SwitchNestingLevel, SwitchNestingLevel);

        return true;
    }

    bool TraverseIfStmt(clang::IfStmt* Node) {

        // We only want to check the indentation of the
        // 'if' token if it is not part of an else branch
        auto ParentNodes = Context->getParents(*Node);
        const auto* ParentIfStmt = ParentNodes[0].get<clang::IfStmt>();
        auto IsElseBranch = false;

        if (ParentIfStmt) {
            if (ParentIfStmt->getThen()->getBeginLoc() == Node->getBeginLoc()) {
                // We are the if branch of our parent
                CheckStatementIndentation(Node->getBeginLoc(), Context,
                        CurrentNestingLevel + SwitchNestingLevel);
            } else if (ParentIfStmt->hasElseStorage()) {
                IsElseBranch = true;
            }
        } else {
            // We are the top of an if/else-if chain
            CheckStatementIndentation(Node->getBeginLoc(), Context,
                    CurrentNestingLevel + SwitchNestingLevel);
        }

        // We only check the 'else' token's indentation if the 'then'
        // statement's body is not a compound statement and the statement has an
        // else component
        if (!llvm::isa<clang::CompoundStmt>(Node->getThen()) &&
                Node->hasElseStorage()) {
            if (IsElseBranch) {
                CheckStatementIndentation(Node->getElseLoc(), Context,
                        CurrentNestingLevel + SwitchNestingLevel - 1);
            } else {
                CheckStatementIndentation(Node->getElseLoc(), Context,
                        CurrentNestingLevel + SwitchNestingLevel);
            }
        }

        // If the 'then' or 'else' components of the if statement are compound
        // statements, we need to check the indentation of the right curly brace
        // '}'
        if (llvm::isa<clang::CompoundStmt>(Node->getThen())) {
            auto* CS = llvm::dyn_cast<clang::CompoundStmt>(Node->getThen());
            if (IsElseBranch) {
                CheckStatementIndentation(CS->getRBracLoc(), Context,
                        CurrentNestingLevel + SwitchNestingLevel - 1);
            } else {
                CheckStatementIndentation(CS->getRBracLoc(), Context,
                        CurrentNestingLevel + SwitchNestingLevel);
            }
        }
        if (Node->hasElseStorage()) {
            if (llvm::isa<clang::CompoundStmt>(Node->getElse())) {
                auto* CS = llvm::dyn_cast<clang::CompoundStmt>(Node->getElse());
                if (IsElseBranch) {
                    CheckStatementIndentation(CS->getRBracLoc(), Context,
                            CurrentNestingLevel + SwitchNestingLevel - 1);
                } else {
                    CheckStatementIndentation(CS->getRBracLoc(), Context,
                            CurrentNestingLevel + SwitchNestingLevel);
                }
            }
        }
        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();

        // We also need to check the if condition's continuation indent
        auto LParenLoc = whitespace::GetNextNonWhitespaceLoc(
                Node->getIfLoc().getLocWithOffset(2), SM);
        
        auto RParenLoc = whitespace::GetPreviousNonWhitespaceLoc(
                Node->getThen()->getBeginLoc().getLocWithOffset(-1), SM);
        
        if (IsElseBranch) {
            CheckSourceRangeContinuationIndent(LParenLoc.getLocWithOffset(1),
                    RParenLoc.getLocWithOffset(-1), Context,
                    CurrentNestingLevel + SwitchNestingLevel - 1,
                    SwitchNestingLevel);
        } else {
            CheckSourceRangeContinuationIndent(LParenLoc.getLocWithOffset(1),
                    RParenLoc.getLocWithOffset(-1), Context,
                    CurrentNestingLevel + SwitchNestingLevel,
                    SwitchNestingLevel);
        }

        if (!IsElseBranch) {
            CurrentNestingLevel++;
        }

        Base::TraverseIfStmt(Node);

        if (!IsElseBranch) {
            CurrentNestingLevel--;
        }

        return true;
    }

    unsigned CurrentNestingLevel = 0;
    unsigned SwitchNestingLevel = 0;
    bool IsInSwitchStatement = false;
    clang::ASTContext* Context;
    clang::Preprocessor& PP;
};

void FunctionBodyIndentChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {

        const auto& SM = *Result.SourceManager;
        if (!SM.isWrittenInMainFile(Node->getLocation())) {
            return;
        }

        CheckStatementIndentation(Node->getBeginLoc(), Result.Context, 0);
        if (!Node->isThisDeclarationADefinition()) {
            return;
        }

        CheckStatementIndentation(Node->getEndLoc(), Result.Context, 0);
        FunctionASTVisitor Visitor(Result.Context, PP);
        Visitor.TraverseDecl(const_cast<clang::FunctionDecl*>(Node));
    }
}

// Checks the indentation of a 'standard' statement. A 'standard'
// statement is one whose indentation relies solely on that of
// its direct surroundings (i.e. unary/binary operators,
// break/continue statements, function calls etc.)
static void CheckStandardStatementIndentation(clang::Stmt* Stmt,
        clang::ASTContext* Context, unsigned int NestingLevel,
        bool IsInSwitchStatement, unsigned int SwitchNestingLevel) {

    auto ParentNodes = Context->getParents(*Stmt);

    if (ParentNodes[0].get<clang::CompoundStmt>()) {
        if (IsInSwitchStatement) {
            CheckStatementIndentation(Stmt->getBeginLoc(), Context,
                    NestingLevel + SwitchNestingLevel);
        } else {
            CheckStatementIndentation(
                    Stmt->getBeginLoc(), Context, NestingLevel);
        }
    } else if (ParentNodes[0].get<clang::CaseStmt>() ||
               ParentNodes[0].get<clang::DefaultStmt>()) {
        // If the statement is in a case statement, we automatically know that
        // we need to check for additional indentation.
        CheckStatementIndentation(Stmt->getBeginLoc(), Context,
                NestingLevel + SwitchNestingLevel);
    }
}

// Checks if the given statement is a loop construct
// (i.e. a for loop, while loop etc.). Returns true if it is,
// else returns false.
static bool StatementIsALoopConstruct(clang::Stmt* Stmt) {
    return llvm::isa<clang::ForStmt>(Stmt) ||
           llvm::isa<clang::WhileStmt>(Stmt) || llvm::isa<clang::DoStmt>(Stmt);
}

// Checks if the given statement is a conditional.
// Returns true if it is, else returns false.
static bool StatementIsAConditional(clang::Stmt* Stmt) {
    return llvm::isa<clang::IfStmt>(Stmt);
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett