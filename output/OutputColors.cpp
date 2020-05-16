/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "OutputColors.hpp"

#include <sstream>

namespace nett {
namespace colors {

static const char* Colors[] = {
        "\033[0m",    // Reset
        "\033[37;1m"  // Bold White
};

std::string Colorize(std::string String, enum TerminalColor Color) {

    std::stringstream Result;
    Result << Colors[Color] << String << Colors[0];

    return Result.str();
}

}  // namespace colors
}  // namespace nett