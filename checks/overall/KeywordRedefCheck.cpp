/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "KeywordRedefCheck.hpp"

#include "../../violations/ViolationManager.hpp"

#include <sstream>

namespace nett {
namespace checks {
namespace overall {

void KeywordRedefChecker::MacroDefined(
        const clang::Token& MacroNameTok, const clang::MacroDirective* MD) {

    auto Loc = MD->getMacroInfo()->getDefinitionLoc();

    if (SM.isWrittenInMainFile(Loc)) {

        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        if (MacroNameTok.getIdentifierInfo()->isKeyword(LangOpts)) {
            std::stringstream ErrMsg;
            ErrMsg << "Do not #define a keyword to something else.";

            GlobalViolationManager.AddViolation(
                    new OverallViolation(File.str(), LineNo, ErrMsg.str()));
        }
    }
}

}  // namespace overall
}  // namespace checks
}  // namespace nett