/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_INPUT_FILEUTILS_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_INPUT_FILEUTILS_HPP

#include <string>

namespace nett {
namespace input {

// Replaces all of the occurrences of SearchStr in String with ReplaceStr.
void ReplaceAll(std::string& String, std::string SearchStr,
        const std::string ReplaceStr);

// Extracts and returns the file name from the given filepath.
// Note that this does not work for files ending with '/'.
std::string ExtractFileName(const std::string FilePath);

}  // namespace input
}  // namespace nett

#endif