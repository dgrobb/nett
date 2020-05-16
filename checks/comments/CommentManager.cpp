/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "CommentManager.hpp"

#include "../../violations/ViolationManager.hpp"

#include <map>
#include <sstream>
#include <string>
#include <unordered_set>

namespace nett {

// In order to determine whether functions have comments at
// either the declaration and definition, we need to keep
// track of the comments we see and their locations in order
// to know whether or not to generate violations and where
// to generate a violation if there is one. Due to the
// calllback-based nature of the clang framework, we use
// this global manager instead of passing it through the
// callbacks.
FunctionCommentManager GlobalFunctionCommentManager;

// Represents a function declaration/definition in the code
class DeclarationMarker {
    public:
    // Creates a new marker with the given name
    DeclarationMarker(std::string DeclName) {
        this->DeclName = DeclName;
        HasComment = false;
        DefinitionFound = false;
    }

    // Sets the location of the definition of the declaration
    // This is to allow correct error reporting.
    void SetDefinitionLocation(std::string FileName, uint LineNo) {
        DefinitionFound = true;
        DefinitionLineNo = LineNo;
        this->FileName = FileName;
    }

    // Returns true if the declaration has a definition, else returns false.
    bool HasDefinition(void) {
        return DefinitionFound;
    }

    // Marks the given declaration has having a comment present.
    void MarkAsCommented(void) {
        HasComment = true;
    }

    // Returns true if the function has a comment somewhere in the
    // source code. Else returns false.
    bool IsCommented(void) {
        return HasComment;
    }

    // Returns the name of the declaration
    std::string GetDeclName(void) {
        return DeclName;
    }

    // Returns the filepath of the function's source file.
    // Note that SetDefinitionLocation() has to be called first.
    std::string GetFileName(void) {
        return FileName;
    }

    // Returns the line number of the definition location.
    // Note that SetDefinitionLocation() has to be called first.
    uint GetLineNumber(void) {
        return DefinitionLineNo;
    }

    private:
    std::string DeclName;
    uint DefinitionLineNo;
    std::string FileName;
    bool HasComment;
    bool DefinitionFound;
};

void CommentManager::MarkDeclNameAsSeen(std::string FuncName) {

    CommentManager::SeenDeclNames.insert(FuncName);
    CommentManager::DeclInformation[FuncName] = new DeclarationMarker(FuncName);
}

bool CommentManager::DeclNameHasBeenSeen(std::string FuncName) {

    return CommentManager::SeenDeclNames.count(FuncName) != 0;
}

void CommentManager::SetDefinitionLocation(
        std::string FuncName, std::string FileName, uint LineNo) {

    if (!CommentManager::DeclNameHasBeenSeen(FuncName)) {
        CommentManager::MarkDeclNameAsSeen(FuncName);
    }
    CommentManager::DeclInformation[FuncName]->SetDefinitionLocation(
            FileName, LineNo);
}

void CommentManager::MarkDeclNameAsCommented(std::string DeclName) {

    CommentManager::DeclInformation[DeclName]->MarkAsCommented();
}

void FunctionCommentManager::GenerateCommentViolations(void) {

    for (auto Marker : DeclInformation) {
        if (Marker.first.compare("main") == 0) {
            // The main function does not need to be commented
            continue;
        }

        if (!Marker.second->IsCommented() && Marker.second->HasDefinition()) {
            std::stringstream ErrMsg;
            ErrMsg << "Functions should be preceded by explanatory comments.";

            auto FileName = Marker.second->GetFileName();
            auto LineNo = Marker.second->GetLineNumber();
            GlobalViolationManager.AddViolation(
                    new CommentsViolation(FileName, LineNo, ErrMsg.str()));
        }
    }
}

}  // namespace nett