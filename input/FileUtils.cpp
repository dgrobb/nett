/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FileUtils.hpp"

namespace nett {
namespace input {

void ReplaceAll(std::string& String, std::string SearchStr,
        const std::string ReplaceStr) {

    auto Pos = String.find(SearchStr);
    auto SearchStrLen = SearchStr.size();

    // Keep replacing the search string until there are no substrings left
    while (Pos != std::string::npos) {
        String.replace(Pos, SearchStrLen, ReplaceStr);
        Pos = String.find(SearchStr, Pos + SearchStrLen);
    }
}

std::string ExtractFileName(const std::string FilePath) {

    auto NamePos = FilePath.find_last_of("/");

    if (NamePos == std::string::npos) {
        return FilePath;  // We only have a filename
    } else {
        return FilePath.substr(NamePos + 1);
    }
}

}  // namespace input
}  // namespace nett