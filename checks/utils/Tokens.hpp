/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_UTILS_TOKENS_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_UTILS_TOKENS_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"

#include <string>

namespace nett {
namespace checks {
namespace utils {

// Returns the string representation of the token at the given source location.
std::string GetTokenSourceString(clang::SourceLocation Loc,
        clang::SourceManager& SM, clang::LangOptions LangOpts);

// Returns the source location at the end of the given statement provided
// that it is the start of a macro.
clang::SourceLocation GetMacroEndLocation(
        clang::Stmt* Stmt, clang::ASTContext* Context, clang::Preprocessor& PP);

clang::SourceLocation GetMacroEndLocation(clang::FieldDecl* Stmt,
        clang::ASTContext* Context, clang::Preprocessor& PP);

// Checks if the given text is whitespace
bool isWhitespace(clang::StringRef Text);

// Checks if the given character is whitespace
bool isWhitespace(char C);

// Returns the kind of token at the given location.
clang::tok::TokenKind getTokenKind(clang::SourceLocation Loc,
        const clang::SourceManager& SM, const clang::ASTContext* Context);

// Finds the location of Value at or after the given starting location
// ignoring comments.
clang::SourceLocation FindCharLocation(clang::SourceLocation StartLoc,
        char Value, clang::SourceManager& SM, clang::ASTContext* Context);

}  // namespace utils
}  // namespace checks
}  // namespace nett

#endif