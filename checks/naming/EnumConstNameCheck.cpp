/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "EnumConstNameCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "NamingStyles.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace naming {

void EnumConstNameChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::EnumConstantDecl>(
                "enumConstDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        auto Name = Node->getName();
        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        // Enum constants need to be UPPER_SNAKE_CASE
        if (!nett::naming::IdentifierFollowsNamingStyle(
                    Name, nett::naming::UPPER_SNAKE_CASE)) {
            std::stringstream ErrMsg;

            ErrMsg << "'" << Name.str()
                   << "' misnamed. Enum constants should be NAMED_LIKE_THIS.";

            GlobalViolationManager.AddInfractedName(Name.str());
            GlobalViolationManager.AddViolation(
                    new NamingViolation(File.str(), LineNo, ErrMsg.str()));
        }
    }
}

}  // namespace naming
}  // namespace checks
}  // namespace nett