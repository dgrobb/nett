/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_FILECONTENTMANAGER_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_FILECONTENTMANAGER_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <map>
#include <string>
#include <vector>

namespace nett {

// The type of entry
enum EntryType {
    ENTRY_FUNC_DECL,
    ENTRY_ENUM_DECL,
    ENTRY_STRUCT_DECL,
    ENTRY_UNION_DECL,
    ENTRY_FUNC_DEFN,
    ENTRY_ENUM_DEFN,
    ENTRY_STRUCT_DEFN,
    ENTRY_UNION_DEFN,
    ENTRY_GLOBAL,
    ENTRY_TYPEDEF,
    ENTRY_COMMENT
};

// Information used to construct a content entry
struct EntryInfo {
    std::string File;
    int StartLineNo;
    int EndLineNo;
    EntryType Type;
    clang::SourceLocation StartLoc;
    clang::SourceLocation EndLoc;
    clang::ASTContext* Context;
};

// A file content entry
struct DefinitionEntry {
    DefinitionEntry(struct EntryInfo Info)
        : StartLineNo(Info.StartLineNo), EndLineNo(Info.EndLineNo),
          Type(Info.Type), StartLoc(Info.StartLoc), EndLoc(Info.EndLoc){};

    int StartLineNo;
    int EndLineNo;
    EntryType Type;
    clang::SourceLocation StartLoc;
    clang::SourceLocation EndLoc;  
};

// Keeps track of declarations/definitions within files
class FileContentManager {
    public:
    FileContentManager() {
    }

    // Adds a new entry into the tracker
    void AddEntry(struct EntryInfo Info);

    // Generates a set of whitespace violations using the
    // current mapping information.
    void GenerateWhitespaceViolations(void);

    // The mapping between files and the definitions they contain
    std::map<std::string, std::vector<DefinitionEntry>> MethodMap;
};

// Constructs an entry for the given node.
struct EntryInfo ConstructFileEntry(const clang::Decl* Node,
        clang::ASTContext* Context, EntryType Type,
        bool SemiColonTerminated = false);

extern FileContentManager GlobalFileContentManager;

}  // namespace nett

#endif