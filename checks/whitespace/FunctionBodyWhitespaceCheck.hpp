/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_WHITESPACE_FUNCTIONBODYWHITESPACECHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_WHITESPACE_FUNCTIONBODYWHITESPACECHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace whitespace {

class FunctionBodyWhitespaceChecker
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
    FunctionBodyWhitespaceChecker(clang::Preprocessor& PP) : PP(PP) {
    }
    virtual void run(
            const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

    private:
    clang::Preprocessor& PP;
};

}  // namespace whitespace
}  // namespace checks
}  // namespace nett

#endif