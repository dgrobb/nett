/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_OUTPUT_OUTPUTCOLORS_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_OUTPUT_OUTPUTCOLORS_HPP

#include <string>

namespace nett {
namespace colors {

enum TerminalColor { COLOR_RESET, COLOR_BOLD_WHITE, COLOR_GREEN, COLOR_ORANGE};

// Returns the given string, wrapped within the terminal
// escape sequence corresponding to the given terminal color.
std::string Colorize(std::string String, enum TerminalColor Color);

}  // namespace colors
}  // namespace nett

#endif