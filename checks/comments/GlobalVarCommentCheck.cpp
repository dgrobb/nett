/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "GlobalVarCommentCheck.hpp"

#include "../../violations/ViolationManager.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace comments {

void GlobalVarCommentChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::VarDecl>("globalVar")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        auto File = SM.getFilename(Loc);
        auto VarLineNo = SM.getExpansionLineNumber(Loc);
        auto* RawComment = Result.Context->getRawCommentForDeclNoCache(Node);
        bool HasNoComment = false;

        if (RawComment) {
            // If there is a comment, we need to make sure it ends just above
            // the variable declaration
            auto CommentLoc = RawComment->getEndLoc();
            auto CommentLineNo = SM.getExpansionLineNumber(CommentLoc);

            if (VarLineNo - CommentLineNo > 1) {
                // The comment is too far away
                HasNoComment = true;
            }
        } else {
            // There is no comment for this variable
            HasNoComment = true;
        }

        if (HasNoComment) {
            std::stringstream ErrMsg;
            ErrMsg << "Global variables should be commented.";
            GlobalViolationManager.AddViolation(
                    new CommentsViolation(File, VarLineNo, ErrMsg.str()));
        }
    }
}

}  // namespace comments
}  // namespace checks
}  // namespace nett