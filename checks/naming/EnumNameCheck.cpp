/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "EnumNameCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "NamingStyles.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace naming {

void EnumNameChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto Name = Node->getName();
        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        if (Name.compare("") == 0) {
            // We have an anonymous enum, so we ignore it
            return;
        }

        // Enum names need to be UpperCamelCase
        if (!nett::naming::IdentifierFollowsNamingStyle(
                    Name, nett::naming::UPPER_CAMEL_CASE)) {
            std::stringstream ErrMsg;

            // We only want to log the error at the enum definition
            if (Node->isCompleteDefinition()) {
                ErrMsg << "'" << Name.str()
                       << "' misnamed. Enums should be NamedLikeThis.";

                GlobalViolationManager.AddInfractedName(Name.str());
                GlobalViolationManager.AddViolation(
                        new NamingViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }
    }
}

}  // namespace naming
}  // namespace checks
}  // namespace nett