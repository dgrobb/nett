/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_COMMENTS_COMMENTMANAGER_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_COMMENTS_COMMENTMANAGER_HPP

#ifndef __NETT_FUNCTION_COMMENT_MANAGER_HPP__
#define __NETT_FUNCTION_COMMENT_MANAGER_HPP__

#include <map>
#include <string>
#include <unordered_set>

namespace nett {

class DeclarationMarker;

// We use the CommentManager to keep track of function declarations
// and their associated comments.
class CommentManager {
    public:
    // Marks the given declaration name as having been seen.
    void MarkDeclNameAsSeen(std::string DeclName);

    // Returns true if the declaration name has been seen, else returns false.
    bool DeclNameHasBeenSeen(std::string DeclName);

    // Sets the location of the definition of the given declaration name.
    void SetDefinitionLocation(
            std::string DeclName, std::string FileName, uint LineNo);

    // Marks the given declaration as having some form of comment present.
    void MarkDeclNameAsCommented(std::string DeclName);

    // Resolves all of the seen declarations, generating comment violations
    // for those which don't have a comment present. We make this a virtual
    // method to allow for variation in how comment violations are generated
    // (if we want to check other types of comment violations in future)
    virtual void GenerateCommentViolations(void) = 0;

    protected:
    std::unordered_set<std::string> SeenDeclNames;
    std::map<std::string, DeclarationMarker*> DeclInformation;
};

class FunctionCommentManager : public CommentManager {
    public:
    void GenerateCommentViolations(void) override;
};

extern FunctionCommentManager GlobalFunctionCommentManager;

}  // namespace nett

#endif
#endif
