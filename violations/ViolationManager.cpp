/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "ViolationManager.hpp"

#include "clang/Frontend/FrontendActions.h"

#include <algorithm>
#include <sstream>

namespace std {
template <> struct hash<nett::Violation> {
    std::size_t operator()(const nett::Violation& Viol) const {
        std::hash<std::string> StringHasher;
        auto MessageHash = StringHasher(Viol.GetMessage());
        auto FileHash = StringHasher(Viol.GetSourceFile());
        return 93 * MessageHash ^ FileHash ^ Viol.GetLineNumber();
    }
};
}  // namespace std

namespace nett {

// Since clang and libtooling use callbacks to perform all
// of the checking operations, we use a global violation
// manager variable since trying to inject one into
// the methods will be painful and bug-prone.
ViolationManager GlobalViolationManager;

void ViolationManager::AddViolation(Violation* Viol) {
    ViolationManager::FoundViolations[Viol->GetSourceFile()].insert(Viol);
    ViolationManager::ViolationLineMapping[Viol->GetSourceFile()].insert(
            Viol->GetLineNumber());
    ViolationManager::ViolationCounts[Viol->Type()]++;
}

void ViolationManager::AddInfractedName(std::string IdentifierName) {
    ViolationManager::InfractedNames.insert(IdentifierName);
}

bool ViolationManager::NameHasBeenInfracted(std::string IdentifierName) {
    return ViolationManager::InfractedNames.count(IdentifierName) != 0;
}

void ViolationManager::SetOutputToFile(void) {
    ViolationManager::OutputToFile = true;
}

bool ViolationManager::WillOutputToFile(void) {
    return ViolationManager::OutputToFile;
}

// When we output violations, we want them in a particular order.
// They are ordered by their filepath, then by line number and
// then by violation type. For whitespace violations with column
// indices, we order them in ascending order. For other violations,
// if they are the same type, we order them by their error message.
bool ViolationComparator(Violation* V1, Violation* V2) {

    int FilePathComparison = V1->GetSourceFile().compare(V2->GetSourceFile());

    if (FilePathComparison == 0) {
        if (V1->GetLineNumber() != V2->GetLineNumber()) {
            // If they are on different lines, we only use that for sorting
            return V1->GetLineNumber() < V2->GetLineNumber();
        }
        if (V1->Type() != V2->Type()) {
            // If they are on the same line, and have different types, we use
            // that instead
            return V1->Type() < V2->Type();
        }

        // If they are on the same line, are whitespace violations,
        // and both violations reference columns, we order them by the column
        // number
        if (V1->Type() == ViolationType::WHITESPACE) {
            auto V1Msg = V1->GetMessage();
            auto V2Msg = V2->GetMessage();
            auto SearchString = std::string("at position ");
            auto V1ColPos = V1Msg.find(SearchString);
            auto V2ColPos = V2Msg.find(SearchString);

            if (V1ColPos != std::string::npos &&
                    V2ColPos != std::string::npos) {
                auto V1EndPos =
                        V1Msg.substr(V1ColPos + SearchString.size()).find(",");
                auto V2EndPos =
                        V2Msg.substr(V2ColPos + SearchString.size()).find(",");
                auto V1ColString =
                        V1Msg.substr(V1ColPos + SearchString.size(), V1EndPos);
                auto V2ColString =
                        V2Msg.substr(V2ColPos + SearchString.size(), V2EndPos);

                auto V1ColNo = std::stoi(V1ColString);
                auto V2ColNo = std::stoi(V2ColString);
                return V1ColNo < V2ColNo;
            }
        }

        // Otherwise, we order them by their messages
        return V1->GetMessage().compare(V2->GetMessage()) < 0;
    }

    // Otherwise compare them by filepath
    return FilePathComparison < 0;
}

std::vector<Violation*> ViolationManager::GetViolationsInFile(
        std::string FilePath) {

    auto Violations = ViolationManager::FoundViolations.find(FilePath);

    if (Violations == ViolationManager::FoundViolations.end()) {
        // If there aren't any violations for the file, we return an empty list
        return std::vector<Violation*>();
    }

    auto ResultList = std::vector<Violation*>();
    for (auto* Viol : Violations->second) {
        ResultList.push_back(Viol);
    }
    std::sort(ResultList.begin(), ResultList.end(), ViolationComparator);

    return ResultList;
}

std::vector<Violation*> ViolationManager::GetViolationsOnLine(
        std::string FilePath, uint LineNo) {

    // First we need to consult the line mapping to see whether we have
    // any infractions to report for the given line
    auto LineMapping = ViolationManager::ViolationLineMapping.find(FilePath);

    if (LineMapping == ViolationManager::ViolationLineMapping.end()) {
        // This file contains no style violations
        return std::vector<Violation*>();
    } else if (LineMapping->second.count(LineNo) == 0) {
        // There are no style violations on this line
        return std::vector<Violation*>();
    }

    // If we do have style violations, we filter them out by line number
    auto ViolList = ViolationManager::GetViolationsInFile(FilePath);
    std::vector<Violation*> FilteredList;

    for (auto* Violation : ViolList) {
        if (Violation->GetLineNumber() == LineNo) {
            FilteredList.push_back(Violation);
        }
    }

    return FilteredList;
}

}  // namespace nett