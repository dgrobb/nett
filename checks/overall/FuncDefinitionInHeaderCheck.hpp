/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_OVERALL_FUNCDEFINITIONINHEADERCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_OVERALL_FUNCDEFINITIONINHEADERCHECK_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace nett {
namespace checks {
namespace overall {

class FuncDefinitionInHeaderChecker
    : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
    virtual void run(
            const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
};

}  // namespace overall
}  // namespace checks
}  // namespace nett

#endif