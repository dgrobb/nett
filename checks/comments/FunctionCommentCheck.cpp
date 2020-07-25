/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FunctionCommentCheck.hpp"

#include "../utils/Tokens.hpp"
#include "../whitespace/FileContentManager.hpp"
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
        const auto& Loc = SM.getFileLoc(Node->getBeginLoc());

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
        }
        if (HasComment) {
            GlobalFunctionCommentManager.MarkDeclNameAsCommented(
                    FuncName.str());
        }

        // Finally, we log the declaration/definition
        nett::EntryInfo Info;
        nett::EntryType Type;
        if (Node->isThisDeclarationADefinition()) {
            Type = nett::EntryType::ENTRY_FUNC_DEFN;
            Info = ConstructFileEntry(Node, Result.Context, Type);
        } else {
            Type = nett::EntryType::ENTRY_FUNC_DECL;
            Info = ConstructFileEntry(Node, Result.Context, Type, true);
        }

        GlobalFileContentManager.AddEntry(Info);
    }
}

}  // namespace comments
}  // namespace checks
}  // namespace nett