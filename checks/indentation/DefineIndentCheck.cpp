/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "DefineIndentCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"

#include <sstream>

namespace nett {
namespace checks {
namespace indentation {

void DefineIndentChecker::MacroDefined(
        const clang::Token& MacroNameTok, const clang::MacroDirective* MD) {

    auto Loc = MD->getMacroInfo()->getDefinitionLoc();
    if (!SM.isWrittenInMainFile(Loc)) {
        return;
    }

    auto LeadingIndentSize =
            clang::Lexer::getIndentationForLine(Loc, SM).size();
    auto File = SM.getFilename(Loc);
    auto LocLineNo = SM.getExpansionLineNumber(Loc);

    auto DefineLoc = Loc;
    while (SM.getSpellingColumnNumber(DefineLoc) != 1) {
        DefineLoc = DefineLoc.getLocWithOffset(-1);
    }

    if (LeadingIndentSize != 0) {
        const auto* DirectiveSourceStart = SM.getCharacterData(
                DefineLoc.getLocWithOffset(LeadingIndentSize));
        std::stringstream TokenString;

        int Size = 0;
        while (DirectiveSourceStart[Size] != ' ') {
            TokenString << DirectiveSourceStart[Size];
            Size++;
        }

        std::stringstream ErrMsg;
        ErrMsg << "'" << TokenString.str() << "' "
               << "Expected indent of 0 spaces, found " << LeadingIndentSize
               << ".";

        GlobalViolationManager.AddViolation(
                new IndentationViolation(File, LocLineNo, ErrMsg.str()));
    }
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett