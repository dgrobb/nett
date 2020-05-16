/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "BracesCheck.hpp"

#include "../../violations/ViolationManager.hpp"

#include "clang/AST/ParentMap.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Preprocessor.h"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace braces {

void CheckBraceIsAtEndOfLine(
        clang::SourceLocation BraceLoc, clang::ASTContext* Context) {

    auto& SM = Context->getSourceManager();
    auto LangOpts = Context->getLangOpts();
    auto NextToken = clang::Lexer::findNextToken(BraceLoc, SM, LangOpts);

    if (SM.getExpansionLineNumber(NextToken->getLocation()) ==
            SM.getExpansionLineNumber(BraceLoc)) {

        auto File = SM.getFilename(BraceLoc);
        auto LineNo = SM.getExpansionLineNumber(BraceLoc);

        std::stringstream ErrMsg;
        ErrMsg << "Opening brace is not the last character on a line.";

        GlobalViolationManager.AddViolation(
                new BracesViolation(File, LineNo, ErrMsg.str()));
    }
}

}  // namespace braces
}  // namespace checks
}  // namespace nett