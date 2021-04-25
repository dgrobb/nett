/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_OVERALL_KEYWORDREDEFCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_OVERALL_KEYWORDREDEFCHECK_HPP

#include "clang/Frontend/FrontendActions.h"

namespace nett {
namespace checks {
namespace overall {

class KeywordRedefChecker : public clang::PPCallbacks {

    public:
    KeywordRedefChecker(clang::SourceManager& SM, clang::LangOptions& LangOpts)
        : SM(SM), LangOpts(LangOpts){};
    virtual void MacroDefined(
            const clang::Token& MacroNameTok, const clang::MacroDirective* MD) override;

    private:
    clang::SourceManager& SM;
    clang::LangOptions& LangOpts;
};

}  // namespace overall
}  // namespace checks
}  // namespace nett

#endif
