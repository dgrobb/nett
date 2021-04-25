/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_ENUMBODYINDENTCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_ENUMBODYINDENTCHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace indentation {

// Checks the indentation of an enum definition.
// Enum definitions should have the following form:
//
// enum {
//     VAL1, VAL2, VAL3,
//     VAL 4, VAL5, ...
// };
//
// There may not be any blank lines within an enum
// definition.
void CheckEnumBodyIndentation(const clang::EnumDecl* Decl,
        clang::ASTContext* Context, unsigned NestingLevel,
        unsigned SwitchNestingLevel);

class EnumBodyIndentChecker
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
    EnumBodyIndentChecker(clang::Preprocessor& PP) : PP(PP) {
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