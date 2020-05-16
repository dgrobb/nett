/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "DefineNameCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "NamingStyles.hpp"

#include <sstream>

namespace nett {
namespace checks {
namespace naming {

void DefineNameChecker::MacroDefined(
        const clang::Token& MacroNameTok, const clang::MacroDirective* MD) {

    if (!SM.isWrittenInMainFile(MD->getLocation())) {
        return;
    }
    Macros.emplace_back(MacroNameTok, MD->getMacroInfo());
}

void DefineNameChecker::EndOfMainFile() {

    // We now have all of the information from the preprocessor
    // in order to perform the naming checks.
    for (const auto& MacroEntry : Macros) {

        const clang::MacroInfo* MI = MacroEntry.second;
        auto Loc = MI->getDefinitionLoc();

        if (SM.isWrittenInMainFile(Loc)) {
            if (MI->isUsedForHeaderGuard()) {
                // We don't want to check header guard naming conventions since
                // they may vary and are not made explicit in the style guide
                return;
            }

            auto Name = MacroEntry.first.getIdentifierInfo()->getName();
            auto File = SM.getFilename(Loc);
            auto LineNo = SM.getExpansionLineNumber(Loc);

            if (!nett::naming::IdentifierFollowsNamingStyle(
                        Name, nett::naming::UPPER_SNAKE_CASE)) {
                if (!GlobalViolationManager.NameHasBeenInfracted(Name)) {
                    std::stringstream ErrMsg;
                    ErrMsg << "'" << Name.str()
                           << "' misnamed. #defines should be NAMED_LIKE_THIS.";

                    GlobalViolationManager.AddInfractedName(Name.str());
                    GlobalViolationManager.AddViolation(
                            new NamingViolation(File, LineNo, ErrMsg.str()));
                }
            }
        }
    }

    // Clear the macros before working on the next file.
    Macros.clear();
}

}  // namespace naming
}  // namespace checks
}  // namespace nett