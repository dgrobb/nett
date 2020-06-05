/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "StructUnionMemberAccessWhitespaceCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "WhitespaceCheck.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace whitespace {

void StructUnionMemberAccessWhitespaceChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::MemberExpr>("memberExpr")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();
        auto File = SM.getFilename(Node->getOperatorLoc());

        if (!SM.isWrittenInMainFile(Node->getOperatorLoc())) {
            return;
        }

        std::string OpString = Node->isArrow() ? "->" : ".";
        auto OpLoc = Node->getOperatorLoc();
        auto OpEndLoc = OpLoc.getLocWithOffset(OpString.size() - 1);

        auto RHSLoc = Node->getMemberLoc();
        auto LHSLoc = Node->getOperatorLoc().getLocWithOffset(-1);
        while (utils::isWhitespace(*SM.getCharacterData(LHSLoc))) {
            LHSLoc = LHSLoc.getLocWithOffset(-1);
        }

        auto OpLineNo = SM.getExpansionLineNumber(OpLoc);
        auto RHSLineNo = SM.getExpansionLineNumber(RHSLoc);
        auto LHSLineNo = SM.getExpansionLineNumber(LHSLoc);

        if (LHSLineNo != OpLineNo) {
            std::stringstream ErrMsg;
            ErrMsg << "Member accesses should look like: X" << OpString << "a";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(), OpLineNo, ErrMsg.str()));
        } else {
            // There should be no spaces to the left
            CheckLocationWhitespace(LHSLoc, OpLoc, 0, SM, LangOpts);   
        }
        if (OpLineNo == RHSLineNo) {
            // There should be no spaces to the right
            CheckLocationWhitespace(OpEndLoc, RHSLoc, 0, SM, LangOpts);
        } 
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett