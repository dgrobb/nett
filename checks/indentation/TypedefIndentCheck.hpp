/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_TYPEDEFINDENTCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_TYPEDEFINDENTCHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace indentation {

// Checks the indentation of a typedef declaration.
// Note that typedefs of structs and enums are ignored
// if they are struct/enum definitions.
void CheckTypedefIndentation(const clang::TypedefDecl* Decl,
        clang::ASTContext* Context, unsigned NestingLevel,
        unsigned SwitchNestingLevel);

class TypedefIndentChecker
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
    TypedefIndentChecker() {
    }

    virtual void run(
            const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
};

}  // namespace indentation
}  // namespace checks
}  // namespace nett

#endif