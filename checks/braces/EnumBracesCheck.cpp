/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "EnumBracesCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "BracesCheck.hpp"

#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace braces {

void EnumBracesChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {

        const auto& SM = *Result.SourceManager;
        auto File = SM.getFilename(Node->getLocation());

        if (!SM.isWrittenInMainFile(Node->getLocation())) {
            return;
        }

        if (!Node->isThisDeclarationADefinition()) {
            return;
        }

        auto EnumLoc = Node->getLocation();
        auto OpenBraceLoc = Node->getBraceRange().getBegin();

        // Check that the brace is on the same line as the name
        if (SM.getExpansionLineNumber(EnumLoc) !=
                SM.getExpansionLineNumber(OpenBraceLoc)) {

            auto LineNo = SM.getExpansionLineNumber(OpenBraceLoc);

            std::stringstream ErrMsg;
            ErrMsg << "Opening braces should look like: enum X {";

            GlobalViolationManager.AddViolation(
                    new BracesViolation(File, LineNo, ErrMsg.str()));
        }

        // Check that the brace is the last thing on the line
        CheckBraceIsAtEndOfLine(OpenBraceLoc, Result.Context);
    }
}

}  // namespace braces
}  // namespace checks
}  // namespace nett