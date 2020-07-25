/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "EnumBodyIndentCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "../utils/Typedef.hpp"
#include "../whitespace/FunctionDefinitionManager.hpp"
#include "IndentCheck.hpp"

#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace indentation {

void EnumBodyIndentChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::EnumDecl>(
                "topLevelEnumDecl")) {

        const auto& SM = *Result.SourceManager;
        auto Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        CheckEnumBodyIndentation(Node, Result.Context, 0, 0);
        
        nett::EntryType Type;
        if (Node->isThisDeclarationADefinition()) {
            Type = nett::EntryType::ENTRY_ENUM_DEFN;
        } else {
            Type = nett::EntryType::ENTRY_ENUM_DECL;
        }
        
        nett::EntryInfo Info = ConstructFileEntry(Node, Result.Context, Type, true);
        GlobalFileContentManager.AddEntry(Info);

    }
}

void CheckEnumBodyIndentation(const clang::EnumDecl* Decl,
        clang::ASTContext* Context, unsigned NestingLevel,
        unsigned SwitchNestingLevel) {

    CheckStatementIndentation(checks::utils::GetTypeDeclBeginLoc(Decl, Context),
            Context, NestingLevel + SwitchNestingLevel);

    if (!Decl->isThisDeclarationADefinition()) {
        CheckSourceRangeContinuationIndent(
                checks::utils::GetTypeDeclBeginLoc(Decl, Context),
                Decl->getEndLoc(), Context, NestingLevel, SwitchNestingLevel);
    } else {
        CheckStatementIndentation(
                Decl->getEndLoc(), Context, NestingLevel + SwitchNestingLevel);

        auto EnumBodyRange = Decl->getBraceRange();
        auto BraceEndLoc = EnumBodyRange.getEnd().getLocWithOffset(-1);
        auto LangOpts = Context->getLangOpts();

        CheckSourceRangeContinuationIndent(EnumBodyRange.getBegin(),
                BraceEndLoc, Context, NestingLevel - 1, SwitchNestingLevel);
    }
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett