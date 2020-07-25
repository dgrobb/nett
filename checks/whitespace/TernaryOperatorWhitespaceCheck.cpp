/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "TernaryOperatorWhitespaceCheck.hpp"

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

void TernaryOperatorWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::ConditionalOperator>(
                "ternaryExpr")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        auto QuestionLoc = Node->getQuestionLoc();
        auto ColonLoc = Node->getColonLoc();
        auto CondEndLoc = GetPreviousNonWhitespaceLoc(
                QuestionLoc.getLocWithOffset(-1), SM);
        auto TrueBeginLoc =
                GetNextNonWhitespaceLoc(QuestionLoc.getLocWithOffset(1), SM);
        auto TrueEndLoc =
                GetPreviousNonWhitespaceLoc(ColonLoc.getLocWithOffset(-1), SM);
        auto FalseBeginLoc =
                GetNextNonWhitespaceLoc(ColonLoc.getLocWithOffset(1), SM);

        if (SM.getExpansionLineNumber(CondEndLoc) ==
                SM.getExpansionLineNumber(QuestionLoc)) {
            CheckLocationWhitespace(CondEndLoc, QuestionLoc, 1, SM, LangOpts);
        }
        if (SM.getExpansionLineNumber(QuestionLoc) ==
                SM.getExpansionLineNumber(TrueBeginLoc)) {
            CheckLocationWhitespace(QuestionLoc, TrueBeginLoc, 1, SM, LangOpts);
        }
        if (SM.getExpansionLineNumber(TrueEndLoc) ==
                SM.getExpansionLineNumber(ColonLoc)) {
            CheckLocationWhitespace(TrueEndLoc, ColonLoc, 1, SM, LangOpts);
        }
        if (SM.getExpansionLineNumber(ColonLoc) ==
                SM.getExpansionLineNumber(FalseBeginLoc)) {
            CheckLocationWhitespace(ColonLoc, FalseBeginLoc, 1, SM, LangOpts);
        }
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett