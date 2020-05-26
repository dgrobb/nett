/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "VarDeclWhitespaceCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "PointerStyles.hpp"
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

void VarDeclWhitespaceChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {

        auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();

        if (!SM.isWrittenInMainFile(Node->getBeginLoc())) {
            return;
        }

        auto StartLoc = Node->getBeginLoc();
        auto TrueStartLoc = Node->getLocation();
        auto EndLoc = SM.getExpansionLoc(Node->getEndLoc());

        // If we have: static const int a = 5, b = 5;
        //             ^                ^
        //             startLoc         trueLoc
        //                                     ^
        //                                     trueLoc
        // If there are no '=' between start and true, then we continue as
        // normal. Otherwise, we need to shift start to the token before true
        bool multiAssign = false;
        auto TestLoc = StartLoc;
        while (TestLoc < TrueStartLoc) {
            if (*SM.getCharacterData(TestLoc) == '=') {
                multiAssign = true;
                break;
            }
            TestLoc = TestLoc.getLocWithOffset(1);
        }
        if (multiAssign) {
            StartLoc = TrueStartLoc;
        }

        if (Node->hasInit()) {
            // We need to shift the end location to before the
            // assignment to allow for proper pointer checks.
            // Note that we need to find the '=' furthest to
            // the left to account for ternary operators.
            auto CurrentLoc = EndLoc;
            while (CurrentLoc >= StartLoc) {
                if (*SM.getCharacterData(CurrentLoc) == '=') {
                    EndLoc = CurrentLoc;
                }
                CurrentLoc = CurrentLoc.getLocWithOffset(-1);
            }
            // while (*SM.getCharacterData(EndLoc) != '=') {
            //     EndLoc = EndLoc.getLocWithOffset(-1);
            // }
            auto AssignOpLoc = EndLoc;
            EndLoc = GetPreviousNonWhitespaceLoc(
                    AssignOpLoc.getLocWithOffset(-1), SM);

            // We also need to check the whitespace around the '=' operator
            auto LHS = GetPreviousNonWhitespaceLoc(
                    AssignOpLoc.getLocWithOffset(-1), SM);
            auto RHS = GetNextNonWhitespaceLoc(
                    AssignOpLoc.getLocWithOffset(1), SM);

            if (SM.getExpansionLineNumber(LHS) ==
                    SM.getExpansionLineNumber(AssignOpLoc)) {
                CheckLocationWhitespace(LHS, AssignOpLoc, 1, SM, LangOpts);
            }
            if (SM.getExpansionLineNumber(AssignOpLoc) ==
                    SM.getExpansionLineNumber(RHS)) {
                CheckLocationWhitespace(AssignOpLoc, RHS, 1, SM, LangOpts);
            }

            // We check the right hand side of the assignment for
            // whitespace errors. We ignore pointers to avoid issues
            // with multiplication.
            CheckSourceRangeWhitespaceTokensNoPointers(
                    RHS, Node->getEndLoc(), SM, LangOpts);
        }
        CheckSourceRangeWhitespaceTokens(StartLoc, EndLoc, SM, LangOpts);
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett