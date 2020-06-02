/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_FUNCTIONDEFINITIONMANAGER_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_FUNCTIONDEFINITIONMANAGER_HPP

#include <string>
#include <map>
#include <vector>

namespace nett {

// An method definition entry
struct DefinitionEntry {
    DefinitionEntry(std::string FuncName_, int StartLineNo_, int EndLineNo_) : 
            FuncName(FuncName_), StartLineNo(StartLineNo_), EndLineNo(EndLineNo_){};
            
    std::string FuncName;
    int StartLineNo;
    int EndLineNo;
};

// Keeps track of method definition locations
class FunctionDefinitionManager {
    public:
    FunctionDefinitionManager() {}

    // Adds a new function definition mapping to the tracker
    void AddDefinition(std::string File, std::string FuncName, 
            int StartLineNo, int EndLineNo);

    // Generates a set of whitespace violations using the 
    // current mapping information.
    void GenerateWhitespaceViolations(void);
    
    // The mapping between files and the definitions they contain
    std::map<std::string, std::vector<DefinitionEntry>> MethodMap;
};

extern FunctionDefinitionManager GlobalFunctionDefinitionManager;

}  // namespace nett

#endif