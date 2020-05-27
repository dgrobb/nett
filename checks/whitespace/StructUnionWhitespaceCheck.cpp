/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "StructUnionWhitespaceCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "../utils/Typedef.hpp"
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

void StructUnionWhitespaceChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::RecordDecl>("recordDecl")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();
        auto File = SM.getFilename(Node->getBeginLoc());

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        // If the record has a typedef, there should be a single space
        // between 'typedef' and 'struct/union' and the two should be on
        // the same line.
        auto TypedefBeginLoc =
                checks::utils::GetTypeDeclBeginLoc(Node, Result.Context);
        auto RecordBeginLoc = Node->getBeginLoc();

        if (RecordBeginLoc != RecordBeginLoc) {
            // We have a typedef
            if (SM.getExpansionLineNumber(RecordBeginLoc) !=
                    SM.getExpansionLineNumber(TypedefBeginLoc)) {
                std::stringstream ErrMsg;
                if (Node->isUnion()) {
                    ErrMsg << "Typedef'd unions should look like: typedef "
                              "union ...";
                } else {
                    ErrMsg << "Typedef'd structs should look like: typedef "
                              "struct ...";
                }
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File.str(), SM.getExpansionLineNumber(RecordBeginLoc),
                        ErrMsg.str()));
            } else {
                auto TypedefEndLoc = TypedefBeginLoc.getLocWithOffset(6);
                CheckLocationWhitespace(
                        TypedefEndLoc, RecordBeginLoc, 1, SM, LangOpts);
            }
        }

        // We need to deal with when we have anonymous structs/unions
        bool isAnonymous = Node->getName().compare("") == 0;
        auto RecordEndLoc = RecordBeginLoc;
        if (Node->isUnion()) {
            RecordEndLoc = RecordEndLoc.getLocWithOffset(4);
        } else {
            RecordEndLoc = RecordEndLoc.getLocWithOffset(5);
        }

        if (!isAnonymous) {
            // The name of the record, and the 'struct/union' keyword should be
            // on the same line and single space separated
            auto RecordName =
                    clang::Lexer::findNextToken(RecordEndLoc, SM, LangOpts);
            auto RecordNameStartLoc = RecordName->getLocation();

            if (SM.getExpansionLineNumber(RecordEndLoc) !=
                    SM.getExpansionLineNumber(RecordNameStartLoc)) {
                std::stringstream ErrMsg;
                if (Node->isUnion()) {
                    ErrMsg << "Unions should look like: union X";
                } else {
                    ErrMsg << "Structs should look like: struct X";
                }
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File.str(), SM.getExpansionLineNumber(RecordEndLoc),
                        ErrMsg.str()));
            } else {
                CheckLocationWhitespace(
                        RecordEndLoc, RecordNameStartLoc, 1, SM, LangOpts);
            }

            // The name of the enum and the open curly brace should be
            // on the same line and be single space separated
            if (Node->isThisDeclarationADefinition()) {
                auto RecordNameEndLoc = RecordNameStartLoc.getLocWithOffset(
                        RecordName->getLength() - 1);
                auto OpenBraceLoc = utils::FindCharLocation(
                        RecordNameEndLoc, '{', SM, Result.Context);

                if (SM.getExpansionLineNumber(RecordNameEndLoc) ==
                        SM.getExpansionLineNumber(OpenBraceLoc)) {
                    CheckLocationWhitespace(
                            RecordNameEndLoc, OpenBraceLoc, 1, SM, LangOpts);
                }
            }
        } else {
            // The 'struct/union' keyword and the open curly brace should be
            // on the same line and be single space separated
            if (Node->isThisDeclarationADefinition()) {
                auto OpenBraceLoc = utils::FindCharLocation(
                        RecordEndLoc, '{', SM, Result.Context);
                if (SM.getExpansionLineNumber(RecordEndLoc) ==
                        SM.getExpansionLineNumber(OpenBraceLoc)) {
                    CheckLocationWhitespace(
                            RecordEndLoc, OpenBraceLoc, 1, SM, LangOpts);
                }
            }
        }

        // Now, we check the semi-colon position for each field within the enum
        // (we skip any fields which are structs/enums/unions)
        if (Node->isThisDeclarationADefinition()) {
            for (auto Field : Node->fields()) {
                if (Field->getType()->getAs<clang::RecordType>() ||
                        Field->getType()->getAs<clang::EnumType>()) {
                    continue;
                }

                auto SemiLoc = utils::FindCharLocation(
                        Field->getEndLoc(), ';', SM, Result.Context);
                auto FieldEndLoc = GetPreviousNonWhitespaceLoc(
                        SemiLoc.getLocWithOffset(-1), SM);

                if (SM.getExpansionLineNumber(SemiLoc) !=
                        SM.getExpansionLineNumber(FieldEndLoc)) {
                    std::stringstream ErrMsg;
                    ErrMsg << "Semicolons should be on the same line as the "
                              "end of a declaration.";
                    GlobalViolationManager.AddViolation(new WhitespaceViolation(
                            File.str(), SM.getExpansionLineNumber(SemiLoc),
                            ErrMsg.str()));
                } else {
                    CheckLocationWhitespace(
                            FieldEndLoc, SemiLoc, 0, SM, LangOpts);
                }

                CheckSourceRangeWhitespaceTokens(
                        Field->getBeginLoc(), Field->getEndLoc(), SM, LangOpts);
            }
        }

        // Finally, we check that the ending semi-colon is directly after the
        // end of the record
        auto SemiLoc = utils::FindCharLocation(
                Node->getEndLoc(), ';', SM, Result.Context);
        auto EndLoc =
                GetPreviousNonWhitespaceLoc(SemiLoc.getLocWithOffset(-1), SM);

        if (SM.getExpansionLineNumber(SemiLoc) !=
                SM.getExpansionLineNumber(EndLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Semicolons should be on the same line as the end of a "
                      "declaration.";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(SemiLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(EndLoc, SemiLoc, 0, SM, LangOpts);
        }
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett