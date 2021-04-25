/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_NAMING_DEFINENAMECHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_NAMING_DEFINENAMECHECK_HPP

#ifndef __NETT_NAMING_DEFINE_NAME_CHECK__
#define __NETT_NAMING_DEFINE_NAME_CHECK__

#include "clang/Frontend/FrontendActions.h"

namespace nett {
namespace checks {
namespace naming {

class DefineNameChecker : public clang::PPCallbacks {

    public:
    DefineNameChecker(clang::SourceManager& Mgr) : SM(Mgr){};
    virtual void MacroDefined(
            const clang::Token& MacroNameTok, const clang::MacroDirective* MD) override;
    virtual void EndOfMainFile() override;

    private:
    clang::SourceManager& SM;
    std::vector<std::pair<clang::Token, const clang::MacroInfo*>> Macros;
};

#endif

}  // namespace naming
}  // namespace checks
}  // namespace nett
#endif
