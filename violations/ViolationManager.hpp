/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_VIOLATIONS_VIOLATIONMANAGER_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_VIOLATIONS_VIOLATIONMANAGER_HPP

#include "Violation.hpp"

#include <map>
#include <unordered_set>
#include <vector>

namespace nett {

struct ViolationEqualityChecker {
    public:
    bool operator()(const Violation* V1, const Violation* V2) const {
        return *V1 == *V2;
    }
};

struct ViolationHasher {
    public:
    size_t operator()(const Violation* Viol) const {
        std::hash<std::string> StringHasher;
        auto MessageHash = StringHasher(Viol->GetMessage());
        auto FileHash = StringHasher(Viol->GetSourceFile());
        return 93 * MessageHash ^ FileHash ^ Viol->GetLineNumber();
    }
};

class ViolationManager {
    public:
    // Adds a new violation into the violation manager
    void AddViolation(Violation* Violation);

    // Adds the given identifier name into the violation manager's
    // list of known naming infractions. This allows us to depict
    // whether a NOTE violation needs to be added instead of
    // a NAMING violation.
    void AddInfractedName(std::string IdentfierName);

    // Checks if the given identifier name has been infracted.
    // Returns true if it has, else returns false.
    bool NameHasBeenInfracted(std::string IdentiferName);

    // Returns a sorted list of violations that have been generated for
    // the given file. The violations in this list are sorted
    // in ascending order by line number for simpler printing.
    std::vector<Violation*> GetViolationsInFile(std::string FilePath);

    // Returns a list of violations that have been generated on
    // the given line of the given file.
    std::vector<Violation*> GetViolationsOnLine(
            std::string FilePath, uint LineNo);

    // Sets that the violations will be output to a file.
    // This allows us to turn off certain messages when sending
    // output to the terminal.
    void SetOutputToFile(void);

    // Returns whether or not violations will be output to a file.
    // Returns true if they are, else false.
    bool WillOutputToFile(void);

    private:
    std::map<std::string, std::unordered_set<Violation*, ViolationHasher,
                                  ViolationEqualityChecker>>
            FoundViolations;
    std::map<std::string, std::unordered_set<int>> ViolationLineMapping;
    std::unordered_set<std::string> InfractedNames;
    std::map<ViolationType, uint> ViolationCounts;
    bool OutputToFile = false;
};

extern ViolationManager GlobalViolationManager;

}  // namespace nett

#endif