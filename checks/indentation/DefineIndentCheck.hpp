/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_DEFINEINDENTCHECK_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_INDENTATION_DEFINEINDENTCHECK_HPP

#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/PPCallbacks.h"

namespace nett {
namespace checks {
namespace indentation {

class DefineIndentChecker : public clang::PPCallbacks {

    public:
    DefineIndentChecker(clang::SourceManager& Mgr, clang::Preprocessor& PP)
        : SM(Mgr), PP(PP){};
    virtual void MacroDefined(
            const clang::Token& MacroNameTok, const clang::MacroDirective* MD);

    private:
    clang::SourceManager& SM;
    clang::Preprocessor& PP;
};

}  // namespace indentation
}  // namespace checks
}  // namespace nett

#endif
