/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "Typedef.hpp"

#include "Tokens.hpp"

#include "clang/Lex/Lexer.h"

#include <sstream>

namespace nett {
namespace checks {
namespace utils {

// Checks if the given type declaration is preceded by the
// 'typedef' keyword. Returns true if it is typedef'd, else
// returns false.
static bool TypeDeclIsATypedef(
        const clang::TypeDecl* Decl, clang::ASTContext* Context) {

    auto& SM = Context->getSourceManager();
    auto LangOpts = Context->getLangOpts();
    auto CurrentLoc = Decl->getBeginLoc().getLocWithOffset(-1);

    // Move to the previous token. If we reach the start of the file, we
    // cannot have a typedef
    while (checks::utils::isWhitespace(*SM.getCharacterData(CurrentLoc))) {
        CurrentLoc = CurrentLoc.getLocWithOffset(-1);
        if (CurrentLoc.isInvalid()) {
            // We are at the start of the file, so this is not a typedef
            return false;
        }
    }

    // Now we check if the previous token is 'typedef'
    auto TokenStartLoc =
            clang::Lexer::GetBeginningOfToken(CurrentLoc, SM, LangOpts);
    return checks::utils::GetTokenSourceString(TokenStartLoc, SM, LangOpts)
                   .compare("typedef") == 0;
}

clang::SourceLocation GetTypeDeclBeginLoc(
        const clang::TypeDecl* Decl, clang::ASTContext* Context) {

    if (TypeDeclIsATypedef(Decl, Context)) {
        auto& SM = Context->getSourceManager();
        auto LangOpts = Context->getLangOpts();
        auto CurrentLoc = Decl->getBeginLoc().getLocWithOffset(-1);

        while (checks::utils::isWhitespace(*SM.getCharacterData(CurrentLoc))) {
            CurrentLoc = CurrentLoc.getLocWithOffset(-1);
        }

        return clang::Lexer::GetBeginningOfToken(CurrentLoc, SM, LangOpts);
    }
    return Decl->getBeginLoc();
}

}  // namespace utils
}  // namespace checks
}  // namespace nett