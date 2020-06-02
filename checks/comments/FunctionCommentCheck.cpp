/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FunctionCommentCheck.hpp"
#include "../whitespace/FunctionDefinitionManager.hpp"
#include "CommentManager.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace comments {

void FunctionCommentChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getBeginLoc();

        auto File = SM.getFilename(Loc);
        auto FuncLineNo = SM.getExpansionLineNumber(Loc);
        auto FuncName = Node->getName();
        auto* RawComment = Result.Context->getRawCommentForDeclNoCache(Node);
        bool HasComment = true;

        if (RawComment) {
            // If there is a comment, we need to make sure it ends just above
            // the function declaration
            auto CommentLoc = RawComment->getEndLoc();
            auto CommentLineNo = SM.getExpansionLineNumber(CommentLoc);

            if (FuncLineNo - CommentLineNo != 1) {
                // The comment is too far away
                HasComment = false;
            }
        } else {
            // There is no comment for this declaration
            HasComment = false;
        }

        // Once we know the status of the comment, we update the comment
        // manager. This will automatically add violations for us later once all
        // the checks are completed.
        if (!GlobalFunctionCommentManager.DeclNameHasBeenSeen(FuncName.str())) {
            GlobalFunctionCommentManager.MarkDeclNameAsSeen(FuncName.str());
        }

        if (Node->isThisDeclarationADefinition()) {
            GlobalFunctionCommentManager.SetDefinitionLocation(
                    FuncName.str(), File.str(), FuncLineNo);

            if (HasComment) {
                GlobalFunctionDefinitionManager.AddDefinition(File.str(),
                        FuncName.str(), SM.getExpansionLineNumber(RawComment->getBeginLoc()),
                        SM.getExpansionLineNumber(Node->getEndLoc()));
            } else {
                GlobalFunctionDefinitionManager.AddDefinition(File.str(),
                        FuncName.str(), SM.getExpansionLineNumber(Node->getBeginLoc()),
                        SM.getExpansionLineNumber(Node->getEndLoc()));
            }
        }

        if (HasComment) {
            GlobalFunctionCommentManager.MarkDeclNameAsCommented(FuncName.str());
        }
    }
}

}  // namespace comments
}  // namespace checks
}  // namespace nett