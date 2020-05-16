/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_INPUT_FILEINPUT_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_INPUT_FILEINPUT_HPP

#include <string>

namespace nett {
namespace input {

// Returns the sanitized content of the file at the given
// filepath as a single string. Style checks on line length,
// file naming etc. are performed on the file during the
// sanitizing process.
std::string GetSanitizedFileContent(const std::string FilePath);

// Checks if the file at the given filepath can be accessed.
// Returns true if the file is accessible, returns false otherwise.
bool FileCanBeAccessed(std::string FilePath);

}  // namespace input
}  // namespace nett

#endif