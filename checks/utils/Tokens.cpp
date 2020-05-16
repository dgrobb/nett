/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "Tokens.hpp"

#include "clang/Lex/Lexer.h"

#include <sstream>

namespace nett {
namespace checks {
namespace utils {

std::string GetTokenSourceString(clang::SourceLocation Loc,
        clang::SourceManager& SM, clang::LangOptions LangOpts) {

    auto TokenStartLoc = clang::Lexer::GetBeginningOfToken(Loc, SM, LangOpts);
    auto TokenLength =
            clang::Lexer::MeasureTokenLength(TokenStartLoc, SM, LangOpts);
    const auto* TokenSource = SM.getCharacterData(TokenStartLoc);

    std::stringstream TokenString;
    for (unsigned i = 0; i < TokenLength; i++) {
        TokenString << TokenSource[i];
    }

    return TokenString.str();
}

clang::SourceLocation GetMacroEndLocation(clang::Stmt* Stmt,
        clang::ASTContext* Context, clang::Preprocessor& PP) {

    auto& SM = Context->getSourceManager();
    auto LangOpts = Context->getLangOpts();
    auto StartLoc = Stmt->getBeginLoc();
    auto EndLoc = SM.getFileLoc(Stmt->getEndLoc());

    auto Loc = StartLoc;
    clang::FileID FID = SM.getFileID(Loc);
    const clang::SrcMgr::SLocEntry* Entry = &SM.getSLocEntry(FID);

    while (Entry->getExpansion().getExpansionLocStart().isMacroID()) {
        Loc = Entry->getExpansion().getExpansionLocStart();
        FID = SM.getFileID(Loc);
        Entry = &SM.getSLocEntry(FID);
    }

    EndLoc = Entry->getExpansion().getExpansionLocEnd();

    return EndLoc;
}

clang::SourceLocation GetMacroEndLocation(clang::FieldDecl* Stmt,
        clang::ASTContext* Context, clang::Preprocessor& PP) {

    auto& SM = Context->getSourceManager();
    auto LangOpts = Context->getLangOpts();
    auto StartLoc = Stmt->getBeginLoc();
    auto EndLoc = SM.getFileLoc(Stmt->getEndLoc());

    auto Loc = StartLoc;
    clang::FileID FID = SM.getFileID(Loc);
    const clang::SrcMgr::SLocEntry* Entry = &SM.getSLocEntry(FID);

    while (Entry->getExpansion().getExpansionLocStart().isMacroID()) {
        Loc = Entry->getExpansion().getExpansionLocStart();
        FID = SM.getFileID(Loc);
        Entry = &SM.getSLocEntry(FID);
    }

    EndLoc = Entry->getExpansion().getExpansionLocEnd();

    return EndLoc;
}

bool isWhitespace(char C) {
    return C == ' ' || C == '\n' || C == '\r' || C == '\t' || C == '\f' ||
           C == '\v';
}

bool isWhitespace(clang::StringRef Text) {
    for (clang::StringRef::const_iterator I = Text.begin(), E = Text.end();
            I != E; ++I) {
        if (!isWhitespace(*I)) {
            return false;
        }
    }
    return true;
}

clang::tok::TokenKind getTokenKind(clang::SourceLocation Loc,
        const clang::SourceManager& SM, const clang::ASTContext* Context) {

    clang::Token Tok;
    clang::SourceLocation Beginning =
            clang::Lexer::GetBeginningOfToken(Loc, SM, Context->getLangOpts());

    const bool Invalid = clang::Lexer::getRawToken(
            Beginning, Tok, SM, Context->getLangOpts());

    if (Invalid) {
        return clang::tok::NUM_TOKENS;
    }

    return Tok.getKind();
}

clang::SourceLocation FindCharLocation(clang::SourceLocation StartLoc,
        char Value, clang::SourceManager& SM, clang::ASTContext* Context) {

    StartLoc = SM.getFileLoc(StartLoc);
    auto CharLoc = StartLoc;
    while (true) {
        clang::tok::TokenKind TokKind =
                utils::getTokenKind(CharLoc, SM, Context);
        if (TokKind == clang::tok::NUM_TOKENS ||
                TokKind == clang::tok::comment) {
            CharLoc = CharLoc.getLocWithOffset(1);
            continue;
        }
        if (*SM.getCharacterData(CharLoc) == Value) {
            break;
        }
        CharLoc = CharLoc.getLocWithOffset(1);
    }

    return CharLoc;
}

}  // namespace utils
}  // namespace checks
}  // namespace nett