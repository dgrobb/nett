/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_FUNCDECLPARAMCONTINDENTCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_FUNCDECLPARAMCONTINDENTCHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace indentation {

class FuncDeclParamContIndentChecker
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
    virtual void run(
            const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
};

}  // namespace indentation
}  // namespace checks
}  // namespace nett

#endif