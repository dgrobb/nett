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
static const uint MAX_LINE_LENGTH = 79;
static const char* C_DIGRAPHS[] = {"<:", ":>", "<%", "%>", "%:", "%:%:"};
static const char* C_TRIGRAPHS[] = {"\?\?=", "\?\?/", "\?\?'", "\?\?(", "\?\?)",
        "\?\?!", "\?\?<", "\?\?>", "\?\?-"};

// Sanitizes the given content of a file, replacing
// tabs with spaces and removing non-unix line endings.
static void SanitizeFileContent(std::string& Content) {

    ReplaceAll(Content, "\t", TAB_REPLACEMENT);
    ReplaceAll(Content, "\r", "");
}

// Checks the line lengths of the content in the given file.
static void CheckLineLengths(
        const std::string FilePath, const std::string Content) {

    std::istringstream ContentStream(Content);
    std::string Line;
    int LineNo = 1;

    while (std::getline(ContentStream, Line)) {
        if (Line.length() > MAX_LINE_LENGTH) {
            std::stringstream ErrMsg;
            ErrMsg << "Line length of " << Line.length()
                   << " is over the maximum of " << MAX_LINE_LENGTH << ".";
            GlobalViolationManager.AddViolation(
                    new LineLengthViolation(FilePath, LineNo, ErrMsg.str()));
        }
        LineNo++;
    }
}

// Calculates the line number of the given substring within
// the given file content.
static int SubstringLineNumber(char* Substring, const char* Content) {

    int LineNo = 1;
    char* CurrentPos = Substring;

    while (CurrentPos >= Content) {
        if (*CurrentPos == '\n') {
            LineNo++;
        }
        CurrentPos--;
    }
    return LineNo;
}

// Checks if the given substring is inside a single line comment (//)
// within the given file content. Returns true if it is,
// else returns false.
static bool SubstringInSingleLineComment(char* Substring, const char* Content) {

    // First, we need to get the start and end of the current line
    // of Content in which Substring occurs.
    char* LineStart = Substring;
    char* LineEnd = Substring;

    while (*(LineStart) != '\n' && LineStart > Content) {
        LineStart--;
    }
    while (*(LineEnd) != '\n' && *(LineEnd) != '\0') {
        LineEnd++;
    }

    // Then we find where the next comment occurs
    char* Comment = strstr(LineStart, "//");
    if (Comment == NULL || Comment > LineEnd) {
        return false;
    }

    // If the comment is before the substring, then the
    // substring is commented.
    return Substring >= Comment;
}

// Checks if the given substring is inside a multi-line
// comment (/* ... */) within the given file content.
// Returns true if it is, else returns false.
static bool SubstringInMultiLineComment(char* Substring, const char* Content) {

    // First, we need to get the first occurrence of
    // the end of a multi-line comment (*/) after the
    // substring
    char* CommentEnd = Substring;
    while (!(*(CommentEnd - 1) == '*' && *(CommentEnd) == '/')) {
        if (*(CommentEnd) == '\0') {
            return false;
        }
        CommentEnd++;
    }

    // Here, we've found the end of a multi-line comment.
    // Now we find its sibling /**/
    char* CommentStart = CommentEnd - 3;
    while (!(*(CommentStart) == '/' && *(CommentStart + 1) == '*')) {
        if (CommentStart == Content) {
            return false;
        }
        CommentStart--;
    }

    // Now we have both the start and endpoints of
    // the multi-line comment that ends after the
    // given substring. If the substring is inside
    // this comment, it will be within the two addresses
    return CommentStart <= Substring && Substring <= CommentEnd;
}

// Checks if the given substring is inside double quotes ("")
// Returns true if it is, else returns false.
static bool SubstringInDoubleQuotes(char* Substring, const char* Content) {

    // We know that a substring is within double quotes
    // if there is an odd number of double quotes
    // preceding it in the file.
    int QuoteCount = 0;
    auto StartLoc = Substring;

    while (StartLoc > Content) {
        if (*StartLoc == '"') {
            QuoteCount++;
        }
        StartLoc--;
    }

    return QuoteCount % 2 == 1;
}

// Checks the given file content for digraphs and trigraphs.
static void CheckDigraphsAndTrigraphs(
        const std::string FilePath, const std::string Content) {

    // We need to find the locations of each of the di/trigraphs
    // within the file content. For each *-graph, we need to
    // check if it is contained within a comment or string
    // and ignore it
    // if that is the case.
    char* String = new char[Content.length() + 1];
    std::strcpy(String, Content.c_str());
    std::vector<char*> NGraphsFound;

    for (const auto* Digraph : C_DIGRAPHS) {
        char* CurrentLocation = String;
        char* DigraphLoc;

        while ((DigraphLoc = strstr(CurrentLocation, Digraph))) {
            bool ShouldIgnore =
                    SubstringInSingleLineComment(DigraphLoc, String) ||
                    SubstringInMultiLineComment(DigraphLoc, String) ||
                    SubstringInDoubleQuotes(DigraphLoc, String);

            if (!ShouldIgnore) {
                NGraphsFound.push_back(DigraphLoc);
            }
            CurrentLocation = DigraphLoc + 1;
        }
    }

    for (const auto* Trigraph : C_TRIGRAPHS) {
        char* CurrentLocation = String;
        char* TrigraphLoc;

        while ((TrigraphLoc = strstr(CurrentLocation, Trigraph))) {
            bool ShouldIgnore =
                    SubstringInSingleLineComment(TrigraphLoc, String) ||
                    SubstringInMultiLineComment(TrigraphLoc, String) ||
                    SubstringInDoubleQuotes(TrigraphLoc, String);

            if (!ShouldIgnore) {
                NGraphsFound.push_back(TrigraphLoc);
            }
            CurrentLocation = TrigraphLoc + 1;
        }
    }

    for (auto NGraph : NGraphsFound) {
        std::stringstream ErrMsg;
        ErrMsg << "Digraphs and Trigraphs should not be used.";
        auto LineNo = SubstringLineNumber(NGraph, String);

        GlobalViolationManager.AddViolation(
                new WarningViolation(FilePath, LineNo, ErrMsg.str()));
    }
}

std::string GetSanitizedFileContent(const std::string FilePath) {

    // Read the content of the file
    std::ifstream File(FilePath);
    std::stringstream Buffer;
    Buffer << File.rdbuf();
    std::string Content = Buffer.rdbuf()->str();

    // Perform any required replacements
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

    CheckLineLengths(FilePath, Content);
    CheckDigraphsAndTrigraphs(FilePath, Content);

    return Content;
}

bool FileCanBeAccessed(std::string FilePath) {

    std::ifstream File(FilePath);
    return File.is_open();
}

}  // namespace input
}  // namespace nett