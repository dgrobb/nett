/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FuncDeclParamContIndentCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "IndentCheck.hpp"
#include "IndentWidths.hpp"

#include "clang/Lex/Lexer.h"

#include <sstream>
#include <unordered_set>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace indentation {

void FuncDeclParamContIndentChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {

        const auto& SM = *Result.SourceManager;
        auto LangOpts = Result.Context->getLangOpts();

        if (!SM.isInMainFile(Node->getLocation())) {
            return;
        }

        // The start of the source range is the '(' token after the function
        // name The end of the source range is the ')' token after the final
        // parameter
        auto ParamStartLoc = Node->getLocation();
        auto NextToken = clang::Token();
        while (NextToken.getKind() != clang::tok::l_paren) {
            ParamStartLoc = ParamStartLoc.getLocWithOffset(1);
            NextToken = clang::Lexer::findNextToken(ParamStartLoc, SM, LangOpts)
                                .getValue();
        }
        ParamStartLoc = NextToken.getLocation();

        auto ParamEndLoc = ParamStartLoc;
        if (Node->getNumParams() == 0) {
            // We should get the closing ')' if there are no parameters
            ParamEndLoc = clang::Lexer::findNextToken(ParamEndLoc, SM, LangOpts)
                                  ->getLocation();
        } else {
            const auto* FinalParam =
                    Node->getParamDecl(Node->getNumParams() - 1);
            ParamEndLoc = clang::Lexer::findNextToken(
                    FinalParam->getEndLoc(), SM, LangOpts)
                                  ->getLocation();
        }

        CheckSourceRangeContinuationIndent(
                ParamStartLoc, ParamEndLoc, Result.Context, 0, 0);
    }
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett