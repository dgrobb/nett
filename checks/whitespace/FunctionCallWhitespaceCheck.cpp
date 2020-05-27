/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FunctionCallWhitespaceCheck.hpp"

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

void FunctionCallWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::CallExpr>("funcCall")) {

        auto& SM = *Result.SourceManager;
        auto File = SM.getFilename(Node->getBeginLoc());
        auto LangOpts = Result.Context->getLangOpts();

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        auto LParenLoc = utils::FindCharLocation(
                Node->getBeginLoc(), '(', SM, Result.Context);
        auto RParenLoc = Node->getRParenLoc();
        auto NameEndLoc =
                GetPreviousNonWhitespaceLoc(LParenLoc.getLocWithOffset(-1), SM);

        // The open parenthesis should be on the same line as the function call
        // name if they are on the same line, there should be no spaces
        if (SM.getExpansionLineNumber(NameEndLoc) !=
                SM.getExpansionLineNumber(LParenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Function calls should look like: func_call(...";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(NameEndLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(NameEndLoc, LParenLoc, 0, SM, LangOpts);
        }

        // The first and last parameters should be on the same line
        // as the opening and closing parentheses and there should
        // be no spaces separating them.
        CheckParenWhitespace(LParenLoc, RParenLoc, SM, LangOpts);

        // Finally, we check the comma spacing within the parentheses
        CheckSourceRangeCommaWhitespace(LParenLoc, RParenLoc, SM, LangOpts);
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett