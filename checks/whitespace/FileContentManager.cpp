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

    clang::SourceLocation StartLoc = Node->getBeginLoc();
    clang::SourceLocation EndLoc = Node->getEndLoc();

    if (SemiColonTerminated) {
        EndLoc = checks::utils::FindCharLocation(
                Node->getEndLoc(), ';', SM, Context);
    }
    EndLine = SM.getExpansionLineNumber(EndLoc);

    if (RawComment) {
        int CommentLine = SM.getExpansionLineNumber(RawComment->getBeginLoc());
        if (CommentLine < StartLine) {
            StartLine = CommentLine;
            StartLoc = RawComment->getBeginLoc();
        }
    }

    struct EntryInfo Result = {
            File.str(), StartLine, EndLine, Type, StartLoc, EndLoc, Context};
    return Result;
}

void FileContentManager::AddEntry(struct EntryInfo Info) {

    if (MethodMap.find(Info.File) == MethodMap.end()) {
        MethodMap[Info.File] = std::vector<DefinitionEntry>();
    }
    MethodMap[Info.File].emplace_back(Info);

    // We need to add all of the comments in the file as entries as
    // well so that they can be taken into account later.
    auto& SM = Info.Context->getSourceManager();
    auto CommentList = Info.Context->Comments.getCommentsInFile(
            SM.getFileID(Info.StartLoc));

    if (CommentList) {
        std::map<unsigned int, clang::RawComment*>::const_iterator it;

        for (it = CommentList->begin(); it != CommentList->end(); it++) {
            struct EntryInfo CommentEntry;
            CommentEntry.File = Info.File;
            CommentEntry.StartLineNo =
                    SM.getExpansionLineNumber(it->second->getBeginLoc());
            CommentEntry.EndLineNo =
                    SM.getExpansionLineNumber(it->second->getEndLoc());
            CommentEntry.Type = EntryType::ENTRY_COMMENT;
            CommentEntry.StartLoc = it->second->getBeginLoc();
            CommentEntry.EndLoc = it->second->getEndLoc();
            CommentEntry.Context = Info.Context;
            MethodMap[Info.File].emplace_back(CommentEntry);
        }
    }
}

bool CompareEntries(const DefinitionEntry& a, const DefinitionEntry& b) {
    if (a.StartLineNo == b.StartLineNo) {
        return a.EndLineNo < b.EndLineNo;
    }
    return a.StartLineNo < b.StartLineNo;
}

// Checks if two definition entries cause a separation violation.
// Returns true if a violation is detected, else returns false.
static bool IsSeparationViolation(DefinitionEntry E1, DefinitionEntry E2) {

    auto Diff = E2.StartLineNo - E1.EndLineNo;
    auto SameTypes = E1.Type == E2.Type;
    auto E1IsDecl = E1.Type >= EntryType::ENTRY_ENUM_DECL &&
                    E1.Type <= EntryType::ENTRY_UNION_DECL;
    auto E2IsDecl = E2.Type >= EntryType::ENTRY_ENUM_DECL &&
                    E2.Type <= EntryType::ENTRY_UNION_DECL;

    if (SameTypes && E1.Type == EntryType::ENTRY_GLOBAL) {
        // Global variables don't require spacing
        return Diff > 2;
    } else if (SameTypes && E1.Type == EntryType::ENTRY_TYPEDEF) {
        // Typedefs which aren't struct/enum definitions don't require spacing
        return Diff > 2;
    } else if (SameTypes && E1.Type == EntryType::ENTRY_FUNC_DECL) {
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

        // Once we've sorted the entries, we need to remove
        // comments which are within the bodies of other nodes
        // (i.e. their source locations are within the bounds
        // of their neighbours).
        std::vector<DefinitionEntry> Filtered;
        if (Entries.size() > 0) {
            Filtered.push_back(Entries[0]);
        }
        unsigned curr = 1;
        while (curr < Entries.size()) {
            Filtered.push_back(Entries[curr]);

            unsigned offset = 1;
            while (curr + offset < Entries.size() &&
                    Entries[curr + offset].EndLoc < Entries[curr].EndLoc) {
                // The entry at the offset is nested in the current entry
                offset += 1;
            }
            curr = curr + offset;
        }
        Entries = Filtered;

        // Next, we need to filter out comments which overlap with
        // function comments
        Filtered = std::vector<DefinitionEntry>();
        for (unsigned i = 0; i < Entries.size() - 1; i++) {
            if (Entries[i].Type == EntryType::ENTRY_COMMENT &&
                    Entries[i].StartLineNo == Entries[i + 1].StartLineNo) {
                continue;
            }
            Filtered.push_back(Entries[i]);
        }
        Filtered.push_back(Entries[Entries.size() - 1]);
        Entries = Filtered;

        // Next, we need to move through and check whether the
        // start of the next entry is sufficiently close to the
        // end of the previous
        for (unsigned i = 1; i < Entries.size(); i++) {
            std::string TypeNames[] = {"Functions", "Enums", "Structs",
                    "Unions", "Functions", "Enums", "Structs", "Unions",
                    "Global Variables", "Typedefs", "Comments"};
            std::string E1Type = TypeNames[Entries[i - 1].Type];
            std::string E2Type = TypeNames[Entries[i].Type];

            if (Entries[i].StartLineNo == Entries[i - 1].StartLineNo &&
                    Entries[i].EndLineNo == Entries[i - 1].EndLineNo) {
                // We have the same entry twice (which can happen)
                continue;
            }
            if (Entries[i - 1].Type == EntryType::ENTRY_COMMENT ||
                    Entries[i].Type == EntryType::ENTRY_COMMENT) {
                // We ignore comments
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
