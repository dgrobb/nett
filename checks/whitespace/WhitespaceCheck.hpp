/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_WHITESPACE_WHITESPACECHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_WHITESPACE_WHITESPACECHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"

#include <string>

namespace nett {
namespace checks {
namespace whitespace {

// Generates a new whitespace violation using the provided
// location data. Note that ErrLoc is used solely for
// file and line information.
void GenerateWhitespaceViolation(clang::SourceLocation ErrLoc,
        clang::SourceManager& SM, std::string TokenString, unsigned ColNo,
        unsigned ExpectedSpaces, unsigned ActualSpaces);

// Checks the whitespace between the given start
// and end locations. This method should be used when
// checking whitespace differences for locations
// on the same line in the source code. If the whitespace
// does not match the expected number of spaces, generates
// a whitespace violation.
//
// For example in a if statement:
//     if (cond) {
//             ^ ^
//             | EndLoc
//             |
//             StartLoc
// And in an else statement:
//     } else
//     ^ ^
//     | EndLoc
//     |
//     StartLoc
void CheckLocationWhitespace(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, unsigned ExpectedSpaces,
        clang::SourceManager& SM, clang::LangOptions LangOpts);

// Returns a location corresponding to the previous non-whitespace
// character before the given start location.
clang::SourceLocation GetPreviousNonWhitespaceLoc(
        clang::SourceLocation StartLoc, clang::SourceManager& SM);

// Returns a location corresponding to the next non-whitespace
// character after the given start location.
clang::SourceLocation GetNextNonWhitespaceLoc(
        clang::SourceLocation StartLoc, clang::SourceManager& SM);

// Checks the whitespace within the locations of the given parentheses
void CheckParenWhitespace(clang::SourceLocation LParenLoc,
        clang::SourceLocation RParenLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts);

// Checks the whitespace surrounding commas within the source
// range between StartLoc and EndLoc.
void CheckSourceRangeCommaWhitespace(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts);

// Returns the number of spaces between two source locations.
// If the locations are not on the same line, returns -1 instead.
//
// For example:
// int x = 5
//     ^   ^
//     |   |
//     |   12 (EndLoc)
//     8 (StartLoc)
//      ---  <- 3 spaces
int CountLocationWhitespace(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM);

// Performs whitespace checks (pointer alignment, bracket spacing etc.)
// on tokens within the given two source locations.
void CheckSourceRangeWhitespaceTokens(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts);

// Performs whitespace checks (bracket spacing etc.)
// on tokens within the given two source locations without
// performing pointer checks.
void CheckSourceRangeWhitespaceTokensNoPointers(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts);

}  // namespace whitespace
}  // namespace checks
}  // namespace nett

#endif