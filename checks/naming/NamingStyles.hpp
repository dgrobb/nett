/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_NAMING_NAMINGSTYLES_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_NAMING_NAMINGSTYLES_HPP

#include "clang/Frontend/FrontendActions.h"

namespace nett {
namespace naming {

// The possible naming styles that variable names, type
// names etc. can be according to the style guide.
enum NamingStyle {
    UPPER_CAMEL_CASE,  // UpperCamelCase
    LOWER_CAMEL_CASE,  // lowerCamelCase
    UPPER_SNAKE_CASE,  // UPPER_SNAKE_CASE
    LOWER_SNAKE_CASE,  // lower_snake_case
    FILE_CAMEL_CASE    // fileCamelCase
};

// Checks if the given identifier (variable name, function name etc.)
// follows the given naming style. Returns true if it does else returns false.
bool IdentifierFollowsNamingStyle(llvm::StringRef ID, enum NamingStyle Style);

// Checks if the filename in the provided filepath follows the given
// naming style. Returns true if it does, else returns false.
bool FileNameFollowsNamingStyle(
        llvm::StringRef FilePath, enum NamingStyle Style);

}  // namespace naming
}  // namespace nett

#endif