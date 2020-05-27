/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "StatementSemicolonWhitespaceCheck.hpp"

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

static bool isConstructStatement(const clang::Stmt* Stmt);

void StatementSemicolonWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::Stmt>("stmt")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();
        auto File = SM.getFilename(Node->getBeginLoc());

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        // Constructs (compound statements, loops etc.)
        // don't have semi-colons
        if (isConstructStatement(Node)) {
            return;
        }

        auto ColonLoc = utils::FindCharLocation(
                Node->getEndLoc(), ';', SM, Result.Context);

        // Here, we narrow down our statement matches to only
        // get statements which are direct children of compound
        // statements. This is because children are also matched
        // which causes multiple errors to be generated.
        // Note that we have to check constructs without braces
        // separately for the same reason.
        auto ParentNodes = Result.Context->getParents(*Node);
        if (!ParentNodes[0].get<clang::CompoundStmt>()) {
            return;
        }

        // Now we've found the semi-colon, so we move backward
        auto PrevLoc = ColonLoc.getLocWithOffset(-1);
        while (utils::isWhitespace(*SM.getCharacterData(PrevLoc))) {
            PrevLoc = PrevLoc.getLocWithOffset(-1);
        }

        if (PrevLoc <= Node->getBeginLoc()) {
            // We have an empty statement
            return;
        }

        auto PrevLineNo = SM.getExpansionLineNumber(PrevLoc);
        auto ColonLineNo = SM.getExpansionLineNumber(ColonLoc);

        if (PrevLineNo != ColonLineNo) {
            std::stringstream ErrMsg;
            ErrMsg << "Semicolons should be on the same line as the end of a "
                      "statement.";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(), ColonLineNo, ErrMsg.str()));
        } else {
            CheckLocationWhitespace(PrevLoc, ColonLoc, 0, SM, LangOpts);
        }
    }
}

static bool isConstructStatement(const clang::Stmt* Stmt) {
    return llvm::isa<clang::CompoundStmt>(Stmt) ||
           llvm::isa<clang::IfStmt>(Stmt) || llvm::isa<clang::DoStmt>(Stmt) ||
           llvm::isa<clang::WhileStmt>(Stmt) ||
           llvm::isa<clang::ForStmt>(Stmt) ||
           llvm::isa<clang::CaseStmt>(Stmt) ||
           llvm::isa<clang::SwitchStmt>(Stmt);
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett