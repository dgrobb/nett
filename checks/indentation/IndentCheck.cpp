/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "IndentCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "../whitespace/WhitespaceCheck.hpp"
#include "GlobalVarIndentCheck.hpp"
#include "IndentWidths.hpp"

#include "clang/Lex/Lexer.h"

#include <sstream>
#include <unordered_set>

namespace nett {
namespace checks {
namespace indentation {

void CheckStatementIndentation(clang::SourceLocation Loc,
        clang::ASTContext* Context, unsigned int NestingLevel,
        unsigned int ExpectedIndent /* =0 */) {

    auto& SM = Context->getSourceManager();
    auto File = SM.getFilename(Loc);
    auto LocLineNo = SM.getExpansionLineNumber(Loc);
    auto ActualIndent = SM.getSpellingColumnNumber(Loc) - 1;

    if (ExpectedIndent == 0) {
        ExpectedIndent = INDENT_WIDTH * NestingLevel;
    }

    if (ActualIndent != ExpectedIndent) {
        std::stringstream ErrMsg;
        auto InfractedToken = checks::utils::GetTokenSourceString(
                Loc, SM, Context->getLangOpts());

        if (InfractedToken.compare("") != 0) {
            ErrMsg << "'" << InfractedToken << "' ";
        }
        ErrMsg << "Expected indent of " << ExpectedIndent << " spaces, found "
               << ActualIndent << ".";

        GlobalViolationManager.AddViolation(
                new IndentationViolation(File, LocLineNo, ErrMsg.str()));
    }
}

void CheckSourceRangeContinuationIndent(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::ASTContext* Context,
        unsigned int NestingLevel, unsigned int SwitchNestingLevel) {

    auto& SM = Context->getSourceManager();
    if (SM.getExpansionLineNumber(StartLoc) ==
            SM.getExpansionLineNumber(EndLoc)) {
        return;
    }

    auto LangOpts = Context->getLangOpts();
    auto SourceRange = clang::SourceRange(StartLoc, EndLoc);
    auto CharRange = clang::Lexer::getAsCharRange(SourceRange, SM, LangOpts);
    auto SourceText = clang::Lexer::getSourceText(CharRange, SM, LangOpts);

    // In order to check continuation indentation, we need to check
    // the indentation of tokens at the start of each new line
    auto Newline = std::string("\n");
    auto Offset = SourceText.find(Newline);

    while (Offset != llvm::StringRef::npos) {
        auto NextLineLocation = StartLoc.getLocWithOffset(Offset + 1);
        auto NextTokenLoc = checks::whitespace::GetNextNonWhitespaceLoc(NextLineLocation, SM);
        
        if (SM.getExpansionLineNumber(NextTokenLoc) !=
                SM.getExpansionLineNumber(NextLineLocation)) {
            // The current line is empty
            std::stringstream ErrMsg;
            auto File = SM.getFilename(NextLineLocation);
            auto LineNo = SM.getExpansionLineNumber(NextLineLocation);
            ErrMsg << "Line continuations should not contain blank lines.";

            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File, LineNo, ErrMsg.str()));
        } else {
            if (NextTokenLoc <= EndLoc) {
                CheckStatementIndentation(NextTokenLoc, Context,
                        NestingLevel + SwitchNestingLevel + 2);
            }
        }

        Offset = SourceText.find(Newline, Offset + Newline.size());
    }
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett