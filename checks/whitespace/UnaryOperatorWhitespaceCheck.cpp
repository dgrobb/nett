/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "UnaryOperatorWhitespaceCheck.hpp"

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

void UnaryOperatorWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::UnaryOperator>("unaryOp")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();
        auto File = SM.getFilename(Node->getOperatorLoc());

        if (!SM.isWrittenInMainFile(Node->getOperatorLoc())) {
            return;
        }

        auto OpString = Node->getOpcodeStr(Node->getOpcode()).str();
        auto OpLoc = Node->getOperatorLoc();
        auto OpEndLoc = OpLoc.getLocWithOffset(OpString.size() - 1);
        auto ExprLoc = OpLoc;

        if (Node->isPostfix()) {  // postfix: x++
            ExprLoc = ExprLoc.getLocWithOffset(-1);

            // Move to the left from the operator location until
            // we find something that is not whitespace
            while (utils::isWhitespace(*SM.getCharacterData(ExprLoc))) {
                ExprLoc = ExprLoc.getLocWithOffset(-1);
            }
        } else {  // prefix: ++x
            ExprLoc = ExprLoc.getLocWithOffset(OpString.size());

            // Move to the right from the operator location until
            // we find something that is not whitespace
            while (utils::isWhitespace(*SM.getCharacterData(ExprLoc))) {
                ExprLoc = ExprLoc.getLocWithOffset(1);
            }
        }

        auto OpLineNo = SM.getExpansionLineNumber(OpLoc);
        auto ExprLineNo = SM.getExpansionLineNumber(ExprLoc);

        if (OpLineNo != ExprLineNo) {
            std::stringstream ErrMsg;
            ErrMsg << "Unary operators should look like: ";

            if (Node->isPostfix()) {
                ErrMsg << "X" << OpString;
            } else {
                ErrMsg << OpString << "X";
            }
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(), OpLineNo, ErrMsg.str()));
        } else {
            if (Node->isPostfix()) {  // postfix: x++
                CheckLocationWhitespace(ExprLoc, OpLoc, 0, SM, LangOpts);
            } else {  // prefix: ++x
                CheckLocationWhitespace(OpEndLoc, ExprLoc, 0, SM, LangOpts);
            }
        }
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett