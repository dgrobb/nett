/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_STRUCTUNIONBODYINDENTCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_STRUCTUNIONBODYINDENTCHECK_HPP

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace indentation {

// Checks the indentation of a struct or union.
// Structs and unions should have the following indentation form:
//
// struct <name> {
//     field1;
//     field2;
//     ...
// };
void CheckStructUnionBodyIndentation(const clang::RecordDecl* Decl,
        clang::ASTContext* Context, clang::Preprocessor& PP,
        unsigned NestingLevel, unsigned SwitchNestingLevel);

class StructUnionBodyIndentChecker
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
    StructUnionBodyIndentChecker(clang::Preprocessor& PP) : PP(PP) {
    }

    virtual void run(
            const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

    private:
    clang::Preprocessor& PP;
};

}  // namespace indentation
}  // namespace checks
}  // namespace nett

#endif