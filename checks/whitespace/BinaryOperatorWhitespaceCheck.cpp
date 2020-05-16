/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "BinaryOperatorWhitespaceCheck.hpp"

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

void BinaryOperatorWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::BinaryOperator>("binOp")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();

        if (!SM.isWrittenInMainFile(Node->getOperatorLoc())) {
            return;
        }

        auto OpStartLoc = Node->getOperatorLoc();
        auto OpLineNo = SM.getExpansionLineNumber(OpStartLoc);
        auto OpString = Node->getOpcodeStr();
        auto OpEndLoc =
                Node->getOperatorLoc().getLocWithOffset(OpString.size() - 1);

        auto RHSStartLoc = Node->getRHS()->getBeginLoc();
        auto RHSLineNo = SM.getExpansionLineNumber(RHSStartLoc);

        // We need to find the correct end location of the left hand side of
        // the operator since we can't use LHS->getEndLoc() reliably
        auto LHSEndLoc = Node->getOperatorLoc().getLocWithOffset(-1);
        while (utils::isWhitespace(*SM.getCharacterData(LHSEndLoc))) {
            LHSEndLoc = LHSEndLoc.getLocWithOffset(-1);
        }
        auto LHSLineNo = SM.getExpansionLineNumber(LHSEndLoc);

        if (Node->isCommaOp()) {
            if (LHSLineNo == OpLineNo) {
                // There should be no spaces on the left
                CheckLocationWhitespace(LHSEndLoc, OpStartLoc, 0, SM, LangOpts);
            }
            if (OpLineNo == RHSLineNo) {
                // There should be a single space on the right
                CheckLocationWhitespace(OpEndLoc, RHSStartLoc, 1, SM, LangOpts);
            }
        } else {
            // We need a single space on either side
            if (LHSLineNo == OpLineNo) {
                CheckLocationWhitespace(LHSEndLoc, OpStartLoc, 1, SM, LangOpts);
            }
            if (OpLineNo == RHSLineNo) {
                CheckLocationWhitespace(OpEndLoc, RHSStartLoc, 1, SM, LangOpts);
            }
        }
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett