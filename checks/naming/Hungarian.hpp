/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_NAMING_HUNGARIAN_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_NAMING_HUNGARIAN_HPP

#include "clang/Frontend/FrontendActions.h"

namespace nett {
namespace naming {

// Checks if the given variable name (of the given type) follows Hungarian
// notation. Returns true if it does, else returns false.
bool IsHungarianVarName(llvm::StringRef VarName, clang::QualType VarType);

}  // namespace naming
}  // namespace nett

#endif