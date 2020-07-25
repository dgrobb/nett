/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "EnumWhitespaceCheck.hpp"

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

void EnumWhitespaceChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();
        auto File = SM.getFilename(Node->getBeginLoc());

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        // If the enum has a typedef, there should be a single space
        // between 'typedef' and 'enum' and the two should be on
        // the same line.
        auto TypedefBeginLoc =
                checks::utils::GetTypeDeclBeginLoc(Node, Result.Context);
        auto EnumBeginLoc = Node->getBeginLoc();

        if (TypedefBeginLoc != EnumBeginLoc) {
            // We have a typedef
            if (SM.getExpansionLineNumber(TypedefBeginLoc) !=
                    SM.getExpansionLineNumber(EnumBeginLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Typedef'd enums should look like: typedef enum ...";
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File.str(), SM.getExpansionLineNumber(TypedefBeginLoc),
                        ErrMsg.str()));
            } else {
                auto TypedefEndLoc = TypedefBeginLoc.getLocWithOffset(6);
                CheckLocationWhitespace(
                        TypedefEndLoc, EnumBeginLoc, 1, SM, LangOpts);
            }
        }

        // We need to deal with when we have anonymous enums
        bool isAnonymous = Node->getName().compare("") == 0;
        auto EnumEndLoc = EnumBeginLoc.getLocWithOffset(3);

        if (!isAnonymous) {
            // The name of the enum, and the 'enum' keyword should be
            // on the same line and single space separated
            auto EnumName =
                    clang::Lexer::findNextToken(EnumEndLoc, SM, LangOpts);
            auto EnumNameStartLoc = EnumName->getLocation();

            if (SM.getExpansionLineNumber(EnumEndLoc) !=
                    SM.getExpansionLineNumber(EnumNameStartLoc)) {
                std::stringstream ErrMsg;
                ErrMsg << "Enums should look like: enum X";
                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File.str(), SM.getExpansionLineNumber(EnumEndLoc),
                        ErrMsg.str()));
            } else {
                CheckLocationWhitespace(
                        EnumEndLoc, EnumNameStartLoc, 1, SM, LangOpts);
            }

            // The name of the enum and the open curly brace should be
            // on the same line and be single space separated
            if (Node->isThisDeclarationADefinition()) {
                auto EnumNameEndLoc = EnumNameStartLoc.getLocWithOffset(
                        EnumName->getLength() - 1);
                auto OpenBraceLoc = utils::FindCharLocation(
                        EnumNameEndLoc, '{', SM, Result.Context);

                if (SM.getExpansionLineNumber(EnumNameEndLoc) ==
                        SM.getExpansionLineNumber(OpenBraceLoc)) {
                    CheckLocationWhitespace(
                            EnumNameEndLoc, OpenBraceLoc, 1, SM, LangOpts);
                }
            }
        } else {
            // The 'enum' keyword and the open curly brace should be
            // on the same line and be single space separated
            if (Node->isThisDeclarationADefinition()) {
                auto OpenBraceLoc = utils::FindCharLocation(
                        EnumEndLoc, '{', SM, Result.Context);
                if (SM.getExpansionLineNumber(EnumEndLoc) ==
                        SM.getExpansionLineNumber(OpenBraceLoc)) {
                    CheckLocationWhitespace(
                            EnumEndLoc, OpenBraceLoc, 1, SM, LangOpts);
                }
            }
        }

        // Next, we need to check for comma spacing within the enum body
        if (Node->isThisDeclarationADefinition()) {
            auto BraceRange = Node->getBraceRange();
            CheckSourceRangeCommaWhitespace(
                    BraceRange.getBegin(), BraceRange.getEnd(), SM, LangOpts);
        }

        // Finally, we check that the ending semi-colon is directly after the
        // end of the enum
        auto SemiLoc = utils::FindCharLocation(
                Node->getEndLoc(), ';', SM, Result.Context);
        auto EndLoc =
                GetPreviousNonWhitespaceLoc(SemiLoc.getLocWithOffset(-1), SM);

        if (SM.getExpansionLineNumber(SemiLoc) !=
                SM.getExpansionLineNumber(EndLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Semicolons should be on the same line as the end of a "
                      "declaration.";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(SemiLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(EndLoc, SemiLoc, 0, SM, LangOpts);
        }
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett