/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "StructUnionBodyIndentCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "../utils/Typedef.hpp"
#include "../whitespace/FileContentManager.hpp"
#include "EnumBodyIndentCheck.hpp"
#include "IndentCheck.hpp"

#include "clang/AST/ParentMap.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace indentation {

class RecordASTVisitor : public clang::RecursiveASTVisitor<RecordASTVisitor> {
    using Base = clang::RecursiveASTVisitor<RecordASTVisitor>;

    public:
    explicit RecordASTVisitor(clang::ASTContext* Context,
            clang::Preprocessor& PP, unsigned NestingLevel)
        : Context(Context), PP(PP), CurrentNestingLevel(NestingLevel) {
    }

    bool VisitFieldDecl(clang::FieldDecl* Decl) {

        if (Decl->getType()->getAs<clang::RecordType>() ||
                Decl->getType()->getAs<clang::EnumType>()) {
            return true;
        }

        if (PP.isAtStartOfMacroExpansion(Decl->getBeginLoc())) {
            auto TrueLocation =
                    Context->getSourceManager().getFileLoc(Decl->getBeginLoc());
            CheckStatementIndentation(
                    TrueLocation, Context, CurrentNestingLevel);
            CheckSourceRangeContinuationIndent(TrueLocation,
                    checks::utils::GetMacroEndLocation(Decl, Context, PP),
                    Context, CurrentNestingLevel, 0);
        } else {
            CheckStatementIndentation(
                    Decl->getBeginLoc(), Context, CurrentNestingLevel);
            CheckSourceRangeContinuationIndent(Decl->getBeginLoc(),
                    Decl->getEndLoc(), Context, CurrentNestingLevel, 0);
        }

        return true;
    }

    bool VisitEnumDecl(clang::EnumDecl* Decl) {
        CheckEnumBodyIndentation(Decl, Context, CurrentNestingLevel, 0);
        return true;
    }

    bool TraverseRecordDecl(clang::RecordDecl* Decl) {

        // We need to check the indentation of the start of the
        // struct declaration ('typedef struct' or 'struct')
        CheckStatementIndentation(
                checks::utils::GetTypeDeclBeginLoc(Decl, Context), Context,
                CurrentNestingLevel);

        // We need to check the indentation of the right
        // curly brace since we expect something of the form:
        // } <recordname>; or };
        auto EndBraceLoc = Decl->getBraceRange().getEnd();
        CheckStatementIndentation(EndBraceLoc, Context, CurrentNestingLevel);

        CurrentNestingLevel++;
        Base::TraverseRecordDecl(Decl);
        CurrentNestingLevel--;

        return true;
    }

    clang::ASTContext* Context;
    clang::Preprocessor& PP;
    unsigned CurrentNestingLevel;
};

void StructUnionBodyIndentChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::RecordDecl>(
                "topLevelRecordDecl")) {

        const auto& SM = *Result.SourceManager;
        auto Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        CheckStructUnionBodyIndentation(Node, Result.Context, PP, 0, 0);

        nett::EntryType Type;
        if (Node->isUnion()) {
            if (Node->isThisDeclarationADefinition()) {
                Type = nett::EntryType::ENTRY_UNION_DEFN;
            } else {
                Type = nett::EntryType::ENTRY_UNION_DECL;
            }
        } else {
            if (Node->isThisDeclarationADefinition()) {
                Type = nett::EntryType::ENTRY_STRUCT_DEFN;
            } else {
                Type = nett::EntryType::ENTRY_STRUCT_DECL;
            }
        }

        nett::EntryInfo Info = ConstructFileEntry(Node, Result.Context, Type, true);
        GlobalFileContentManager.AddEntry(Info);
    }
}

void CheckStructUnionBodyIndentation(const clang::RecordDecl* Decl,
        clang::ASTContext* Context, clang::Preprocessor& PP,
        unsigned NestingLevel, unsigned SwitchNestingLevel) {

    if (!Decl->isThisDeclarationADefinition()) {
        // We check struct declarations separately since the definitions
        // are checked during the traversal.
        CheckStatementIndentation(
                checks::utils::GetTypeDeclBeginLoc(Decl, Context), Context,
                NestingLevel + SwitchNestingLevel);
        CheckSourceRangeContinuationIndent(
                checks::utils::GetTypeDeclBeginLoc(Decl, Context),
                Decl->getEndLoc(), Context, NestingLevel, SwitchNestingLevel);
        return;
    }

    RecordASTVisitor Visitor(Context, PP, NestingLevel + SwitchNestingLevel);
    Visitor.TraverseDecl(const_cast<clang::RecordDecl*>(Decl));
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett