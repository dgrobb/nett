/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FileInput.hpp"

#include "../checks/naming/NamingStyles.hpp"
#include "../violations/ViolationManager.hpp"
#include "FileUtils.hpp"

#include <fstream>
#include <sstream>

namespace nett {
namespace input {

static const char TAB_REPLACEMENT[] = "        ";
static const uint MAX_LINE_LENGTH = 80;
static const char* C_DIGRAPHS[] = {"<:", ":>", "<%", "%>", "%:", "%:%:"};
static const char* C_TRIGRAPHS[] = {"\?\?=", "\?\?/", "\?\?'", "\?\?(", "\?\?)",
        "\?\?!", "\?\?<", "\?\?>", "\?\?-"};

// Sanitizes the given content of a file, replacing
// tabs with spaces and removing non-unix line endings.
static void SanitizeFileContent(std::string& Content) {

    ReplaceAll(Content, "\t", TAB_REPLACEMENT);
    ReplaceAll(Content, "\r", "");
}

// Checks if the given string contains a C digraph.
// Returns true if it does, else returns false
static bool ContainsDigraph(const std::string String) {

    for (const auto* digraph : C_DIGRAPHS) {
        if (String.find(digraph) != std::string::npos) {
            return true;
        }
    }
    return false;
}

// Checks if the given string contains a C trigraph.
// Returns true if it does, else returns false
static bool ContainsTrigraph(const std::string String) {

    for (const auto* trigraph : C_TRIGRAPHS) {
        if (String.find(trigraph) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string GetSanitizedFileContent(const std::string FilePath) {

    // Read the content of the file
    std::ifstream File(FilePath);
    std::stringstream Buffer;
    Buffer << File.rdbuf();
    std::string Content = Buffer.rdbuf()->str();

    // Preform any required replacements
    SanitizeFileContent(Content);

    // Check for filename violations
    auto FileName = ExtractFileName(FilePath);
    if (!naming::FileNameFollowsNamingStyle(
                FileName, naming::FILE_CAMEL_CASE)) {
        std::stringstream ErrMsg;
        ErrMsg << "'" << FileName
               << "' misnamed. Files should be namedLikeThis.c";
        GlobalViolationManager.AddViolation(
                new NamingViolation(FilePath, 1, ErrMsg.str()));
    }

    // Check for long lines, digraphs and trigraphs
    std::istringstream ContentStream(Content);
    std::string Line;
    int LineNo = 1;

    while (std::getline(ContentStream, Line)) {
        if (ContainsDigraph(Line) || ContainsTrigraph(Line)) {
            std::stringstream ErrMsg;
            ErrMsg << "Digraphs and Trigraphs should not be used.";
            GlobalViolationManager.AddViolation(
                    new WarningViolation(FilePath, LineNo, ErrMsg.str()));
        }
        if (Line.length() > MAX_LINE_LENGTH) {
            std::stringstream ErrMsg;
            ErrMsg << "Line length of " << Line.length()
                   << " is over the maximum of " << MAX_LINE_LENGTH << ".";
            GlobalViolationManager.AddViolation(
                    new LineLengthViolation(FilePath, LineNo, ErrMsg.str()));
        }
        LineNo++;
    }

    return Content;
}

bool FileCanBeAccessed(std::string FilePath) {

    std::ifstream File(FilePath);
    return File.is_open();
}

}  // namespace input
}  // namespace nett