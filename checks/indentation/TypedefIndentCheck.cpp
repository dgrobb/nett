/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "TypedefIndentCheck.hpp"

#include "IndentCheck.hpp"

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace indentation {

void TypedefIndentChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::TypedefDecl>(
                "topLevelTypedefDecl")) {

        const auto& SM = *Result.SourceManager;
        auto Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        CheckTypedefIndentation(Node, Result.Context, 0, 0);
    }
}

void CheckTypedefIndentation(const clang::TypedefDecl* Decl,
        clang::ASTContext* Context, unsigned CurrentNestingLevel,
        unsigned SwitchNestingLevel) {

    // If the typedef is that of a struct or enum,
    // we ignore it if it is a definition since the indentation
    // is checked elsewhere.
    if (Decl->getUnderlyingType()->isEnumeralType() ||
            Decl->getUnderlyingType()->isRecordType()) {

        const auto* Type = Decl->getUnderlyingType().getTypePtrOrNull();
        if (!Type) {
            // We cannot check anything if there is no type
            return;
        }
        auto* TagDecl = Type->getAsTagDecl();
        if (TagDecl->isThisDeclarationADefinition()) {
            return;
        }
    }

    CheckStatementIndentation(Decl->getBeginLoc(), Context,
            CurrentNestingLevel + SwitchNestingLevel);
    CheckSourceRangeContinuationIndent(Decl->getBeginLoc(), Decl->getEndLoc(),
            Context, CurrentNestingLevel, SwitchNestingLevel);
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett
