/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_UTILS_TYPEDEF_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_UTILS_TYPEDEF_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"

#include <string>

namespace nett {
namespace checks {
namespace utils {

// Returns the starting location of the given type declaration.
// If the type declaration is not a 'typedef', then it is
// equalivalent to Decl->getBeginLoc(), otherwise, it returns
// the location corresponding to the start of the 'typedef'
// specifier. We use this primarily for indentation checks
// with structs and enums.
clang::SourceLocation GetTypeDeclBeginLoc(
        const clang::TypeDecl* Decl, clang::ASTContext* Context);

}  // namespace utils
}  // namespace checks
}  // namespace nett

#endif