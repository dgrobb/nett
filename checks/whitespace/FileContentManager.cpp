/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FileContentManager.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"

#include <algorithm>
#include <sstream>

namespace nett {

FileContentManager GlobalFileContentManager;

struct EntryInfo ConstructFileEntry(const clang::Decl* Node,
        clang::ASTContext* Context, EntryType Type, bool SemiColonTerminated) {

    auto& SM = Context->getSourceManager();
    auto File = SM.getFilename(Node->getBeginLoc());
    auto* RawComment = Context->getRawCommentForDeclNoCache(Node);
    int StartLine = SM.getExpansionLineNumber(Node->getBeginLoc());
    int EndLine;

    if (SemiColonTerminated) {
        EndLine = SM.getExpansionLineNumber(checks::utils::FindCharLocation(
                Node->getEndLoc(), ';', SM, Context));
    } else {
        EndLine = SM.getExpansionLineNumber(Node->getEndLoc());
    }

    if (RawComment) {
        int CommentLine = SM.getExpansionLineNumber(RawComment->getBeginLoc());
        if (CommentLine < StartLine) {
            StartLine = CommentLine;
        }
    }

    struct EntryInfo Result = {File.str(), StartLine, EndLine, Type};
    return Result;
}

void FileContentManager::AddEntry(struct EntryInfo Info) {

    if (MethodMap.find(Info.File) == MethodMap.end()) {
        MethodMap[Info.File] = std::vector<DefinitionEntry>();
    }
    MethodMap[Info.File].emplace_back(Info);
}

bool CompareEntries(const DefinitionEntry& a, const DefinitionEntry& b) {
    return a.StartLineNo < b.StartLineNo;
}

// Checks if two definition entries cause a separation violation.
// Returns true if a violation is detected, else returns false.
static bool IsSeparationViolation(DefinitionEntry E1, DefinitionEntry E2) {

    auto Diff = E2.StartLineNo - E1.EndLineNo;
    auto SameTypes = E1.Type == E2.Type;
    auto E1IsDecl = E1.Type >= nett::EntryType::ENTRY_ENUM_DECL &&
                    E1.Type <= nett::EntryType::ENTRY_UNION_DECL;
    auto E2IsDecl = E2.Type >= nett::EntryType::ENTRY_ENUM_DECL &&
                    E2.Type <= nett::EntryType::ENTRY_UNION_DECL;

    if (SameTypes && E1.Type == nett::EntryType::ENTRY_GLOBAL) {
        // Global variables don't require spacing
        return Diff > 2;
    } else if (SameTypes && E1.Type == nett::EntryType::ENTRY_TYPEDEF) {
        // Typedefs which aren't struct/enum definitions don't require spacing
        return Diff > 2;
    } else if (SameTypes && E1.Type == nett::EntryType::ENTRY_FUNC_DECL) {
        // Function declarations don't require spacing
        return Diff > 2;
    } else if (E1IsDecl && E2IsDecl) {
        // Struct/Union/Enum declarations don't require spacing
        return Diff > 2;
    } else if (Diff != 2) {
        return true;
    }
    return false;
}

void FileContentManager::GenerateWhitespaceViolations(void) {

    for (auto FileEntry : MethodMap) {
        auto File = FileEntry.first;
        auto Entries = FileEntry.second;

        std::sort(Entries.begin(), Entries.end(), CompareEntries);

        // Once we've sorted the entries in the file, we need
        // to move through and check whether the start of the next
        // function is more than one line away from the end
        // of the previous
        for (unsigned i = 1; i < Entries.size(); i++) {
            std::string TypeNames[] = {"Functions", "Enums", "Structs",
                    "Unions", "Functions", "Enums", "Structs", "Unions",
                    "Global Variables", "Typedefs"};
            std::string E1Type = TypeNames[Entries[i - 1].Type];
            std::string E2Type = TypeNames[Entries[i].Type];

            if (Entries[i].StartLineNo == Entries[i - 1].StartLineNo) {
                // We have the same entry twice (which can happen)
                continue;
            }

            if (IsSeparationViolation(Entries[i - 1], Entries[i])) {
                std::stringstream ErrMsg;

                if (E1Type.compare(E2Type) == 0) {
                    ErrMsg << E1Type;
                } else {
                    ErrMsg << E1Type << " and " << E2Type;
                }
                ErrMsg << " should be separated by reasonable whitespace.";

                GlobalViolationManager.AddViolation(new WhitespaceViolation(
                        File, Entries[i - 1].EndLineNo + 1, ErrMsg.str()));
            }
        }
    }
}

}  // namespace nett
