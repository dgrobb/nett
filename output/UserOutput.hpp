/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_OUTPUT_USEROUTPUT_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_OUTPUT_USEROUTPUT_HPP

#include <string>
#include <vector>

namespace nett {
namespace output {

// Prints the style violations found within the given files to the terminal.
void OutputViolationsToTerminal(std::vector<std::string> FilePaths);

// Generates duplicates of the files at the given file paths
// amended with the style violations found within those files.
// Each duplicate is given a '.styled' extension.
void OutputViolationsToFiles(std::vector<std::string> FilePaths);

}  // namespace output
}  // namespace nett

#endif