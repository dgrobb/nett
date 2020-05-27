/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "NamingStyles.hpp"

#include "../../input/FileUtils.hpp"
#include "llvm/Support/Regex.h"

namespace nett {
namespace naming {

static const llvm::Regex Matchers[] = {
        llvm::Regex("^[A-Z][a-zA-Z0-9]*$"),    // UpperCamelCase
        llvm::Regex("^[a-z][a-zA-Z0-9]*$"),    // lowerCamelCase
        llvm::Regex("^[A-Z][A-Z0-9_]*$"),      // UPPER_SNAKE_CASE
        llvm::Regex("^[a-z][a-z0-9_]*$"),      // lower_snake_case
        llvm::Regex("^[a-z0-9][a-zA-Z0-9]*$")  // fileCamelCase <-- allows for
                                               // numbers
};

bool IdentifierFollowsNamingStyle(llvm::StringRef ID, enum NamingStyle Style) {
    return Matchers[Style].match(ID);
}

bool FileNameFollowsNamingStyle(
        llvm::StringRef FilePath, enum NamingStyle Style) {

    std::string ConvertedFilePath = FilePath.str();
    std::string FileName = nett::input::ExtractFileName(ConvertedFilePath);

    // We need to strip off the extension so that we can use our matchers.
    auto ExtPos = FileName.find_first_of(".");
    if (ExtPos != std::string::npos) {
        FileName = FileName.substr(0, ExtPos);
    }
    return Matchers[Style].match(FileName);
}

}  // namespace naming
}  // namespace nett