/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "GlobalVariableSemicolonWhitespaceCheck.hpp"

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

void GlobalVariableSemicolonWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::VarDecl>("globalVar")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();
        auto File = SM.getFilename(Node->getBeginLoc());

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        auto ColonLoc = utils::FindCharLocation(
                Node->getEndLoc(), ';', SM, Result.Context);
        auto ColonLineNo = SM.getExpansionLineNumber(ColonLoc);

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

        if (PrevLineNo != ColonLineNo) {
            std::stringstream ErrMsg;
            ErrMsg << "Semicolons should be on the same line as the end of a "
                      "declaration.";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), ColonLineNo, ErrMsg.str()));
        } else {
            CheckLocationWhitespace(PrevLoc, ColonLoc, 0, SM, LangOpts);
        }
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett