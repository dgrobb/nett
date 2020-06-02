/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FunctionDefinitionManager.hpp"
#include "../../violations/ViolationManager.hpp"

#include <algorithm>
#include <sstream>

namespace nett {

FunctionDefinitionManager GlobalFunctionDefinitionManager;

void FunctionDefinitionManager::AddDefinition(std::string File, 
        std::string FuncName, int StartLineNo, int EndLineNo) {
    
    if (MethodMap.find(File) == MethodMap.end()) {
        MethodMap[File] = std::vector<DefinitionEntry>();
    }

    MethodMap[File].emplace_back(FuncName, StartLineNo, EndLineNo);
}

bool CompareEntries(const DefinitionEntry &a, const DefinitionEntry &b) {
    return a.StartLineNo < b.StartLineNo;
}

void FunctionDefinitionManager::GenerateWhitespaceViolations(void) {

    for (auto FileEntry : MethodMap) {
        auto File = FileEntry.first;
        auto Methods = FileEntry.second;

        std::sort(Methods.begin(), Methods.end(), CompareEntries);

        // Once we've sorted the entries in the file, we need 
        // to move through and check that the start of the next
        // function is more than one line away from the end
        // of the previous
        for (unsigned i = 1; i < Methods.size(); i++) {
            if (Methods[i].StartLineNo - Methods[i - 1].EndLineNo != 2) {
                std::stringstream ErrMsg;
                ErrMsg << "Functions should be separated by reasonable whitespace.";

                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File, Methods[i - 1].EndLineNo, ErrMsg.str()));
            }
        }
    }
}

}  // namespace nett
