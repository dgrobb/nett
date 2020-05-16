/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_BRACES_BRACESCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_BRACES_BRACESCHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace braces {

// Checks if the given brace location occurs at the end of a line.
// If is is not at the end of a line, generates a violation.
void CheckBraceIsAtEndOfLine(
        clang::SourceLocation BraceLoc, clang::ASTContext* Context);

}  // namespace braces
}  // namespace checks
}  // namespace nett

#endif