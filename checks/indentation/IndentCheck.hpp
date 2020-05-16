/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_INDENTCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_INDENTCHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace indentation {

// Checks the indentation of the given source location.
// If the indentation is incorrect, a style violation
// is recorded.
void CheckStatementIndentation(clang::SourceLocation Loc,
        clang::ASTContext* Context, unsigned int NestingLevel,
        unsigned int ExpectedIndent = 0);

// Checks the continuation indentation of the source code within (and
// including) the given start and end locations. The indentation
// of the first line of code within the two locations is ignored.
// If the first token found on a line within has incorrect indentation,
// or an empty line of text is found, a style violation is recorded.
void CheckSourceRangeContinuationIndent(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::ASTContext* Context,
        unsigned int NestingLevel, unsigned int SwitchNestingLevel);

}  // namespace indentation
}  // namespace checks
}  // namespace nett

#endif