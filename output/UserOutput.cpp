/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "UserOutput.hpp"

#include "../violations/ViolationManager.hpp"
#include "OutputColors.hpp"

#include "clang/Tooling/Tooling.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace nett {
namespace output {

#define MAX_VIOLATIONS_PER_CATEGORY_PER_FILE 15
#define MAX_FILEPATH_LENGTH 50
#define STATUS_COLUMN_START 60

// Truncates the given filepath so that it is ready
// for terminal output
std::string TruncateFilePath(std::string FilePath) {

    int startIndex = FilePath.length() - MAX_FILEPATH_LENGTH;
    if (startIndex < 0) {
        startIndex = 0;
    }
    auto NewPath = FilePath.substr(startIndex);
    if (NewPath.length() < FilePath.length()) {
        // We've truncated the path, now we need to trim it
        // up to the first '/', and prepend that with '...'
        auto NewIndex = NewPath.find_first_of('/');
        if (NewIndex != std::string::npos) {
            NewPath = NewPath.substr(NewIndex);
        }
        NewPath.insert(0, "...");
    }

    return NewPath;
}

void OutputViolationsToTerminal(std::vector<std::string> FilePaths) {

    for (auto FilePath : FilePaths) {
        auto TruncatedPath = TruncateFilePath(FilePath);
        auto Padding = STATUS_COLUMN_START - TruncatedPath.length();
        TruncatedPath.append(Padding, ' ');

        llvm::outs() << TruncatedPath;

        auto Violations = GlobalViolationManager.GetViolationsInFile(FilePath);

        if (Violations.empty()) {
            llvm::outs() << colors::Colorize("OK", colors::COLOR_GREEN) << "\n";
            continue;
        } else {
            llvm::outs() << colors::Colorize("ERRORS", colors::COLOR_ORANGE)
                         << "\n";
        }

        for (auto* Violation : Violations) {
            llvm::outs() << Violation->ToTerminalString() << "\n";
        }
    }
}

void OutputViolationsToFiles(std::vector<std::string> FilePaths) {

    for (auto FilePath : FilePaths) {
        std::stringstream NewFilePath;
        NewFilePath << FilePath << ".styled";

        std::ifstream OriginalFile(FilePath);
        std::ofstream NewFile(NewFilePath.str());
        int CurrentLineNo = 1;
        std::string CurrentLineContent;

        std::map<ViolationType, int> ViolationsAddedToFile;
        std::map<ViolationType, bool> ViolationLimitNoted;

        auto ViolationsInFile =
                GlobalViolationManager.GetViolationsInFile(FilePath);
        std::map<ViolationType, int> ViolationTotalsForFile;

        for (auto* Violation : ViolationsInFile) {
            ViolationTotalsForFile[Violation->Type()]++;
        }

        while (std::getline(OriginalFile, CurrentLineContent)) {
            auto ViolationsOnLine = GlobalViolationManager.GetViolationsOnLine(
                    FilePath, CurrentLineNo);
            std::unordered_set<ViolationType> ViolationCategoriesInfracted;

            for (auto* Violation : ViolationsOnLine) {
                auto ViolType = Violation->Type();

                if (ViolationCategoriesInfracted.count(ViolType) > 0) {
                    continue;
                }

                // Don't add a violation if the cap is reached
                if (ViolationsAddedToFile[ViolType] >=
                                MAX_VIOLATIONS_PER_CATEGORY_PER_FILE &&
                        ViolType != ViolationType::NOTE) {
                    continue;
                }

                NewFile << Violation->ToFileString() << "\n";
                ViolationCategoriesInfracted.insert(ViolType);
                ViolationsAddedToFile[ViolType]++;

                // If we reached the cap, and there are more
                // violations of this type, then add a note
                if (ViolationsAddedToFile[ViolType] ==
                                MAX_VIOLATIONS_PER_CATEGORY_PER_FILE &&
                        ViolationTotalsForFile[ViolType] >
                                MAX_VIOLATIONS_PER_CATEGORY_PER_FILE &&
                        ViolType != ViolationType::NOTE) {
                    NewFile << "[NOTE] More violations of this category "
                            << "exist in this file, but the cap has been "
                               "reached.\n";
                }
            }
            NewFile << CurrentLineContent << "\n";
            CurrentLineNo++;
        }

        OriginalFile.close();
        NewFile.close();
    }
}

}  // namespace output
}  // namespace nett
