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

void OutputViolationsToTerminal(std::vector<std::string> FilePaths) {

    for (auto FilePath : FilePaths) {
        llvm::outs() << "Checking "
                     << colors::Colorize(FilePath, colors::COLOR_BOLD_WHITE)
                     << "\n";

        auto Violations = GlobalViolationManager.GetViolationsInFile(FilePath);

        if (Violations.empty()) {
            llvm::outs() << "No style violations detected.\n";
            continue;
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

        std::map<ViolationType, int> FileViolationCounts;
        std::map<ViolationType, bool> ViolationLimitNoted;

        while (std::getline(OriginalFile, CurrentLineContent)) {
            auto ViolationsOnLine = GlobalViolationManager.GetViolationsOnLine(
                    FilePath, CurrentLineNo);
            std::unordered_set<ViolationType> ViolationCategoriesInfracted;

            for (auto* Violation : ViolationsOnLine) {
                auto ViolType = Violation->Type();

                if (ViolationCategoriesInfracted.count(ViolType) > 0) {
                    continue;
                }
                if (FileViolationCounts[ViolType] >
                        MAX_VIOLATIONS_PER_CATEGORY_PER_FILE) {
                    if (!ViolationLimitNoted[ViolType] &&
                            ViolType != ViolationType::NOTE) {
                        NewFile << "[NOTE] More violations of this category "
                                   "exist in this file, but the cap has been "
                                   "reached.\n";
                        ViolationLimitNoted[ViolType] = true;
                    }
                    continue;
                }
                NewFile << Violation->ToFileString() << "\n";
                ViolationCategoriesInfracted.insert(ViolType);
                FileViolationCounts[ViolType]++;
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
