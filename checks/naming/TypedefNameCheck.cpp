/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "TypedefNameCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "NamingStyles.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace naming {

void TypedefNameChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::TypedefDecl>("typedefDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto Name = Node->getName();
        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        // Typedef names need to be UpperCamelCase
        if (!nett::naming::IdentifierFollowsNamingStyle(
                    Name, nett::naming::UPPER_CAMEL_CASE)) {
            std::stringstream ErrMsg;
            ErrMsg << "'" << Name.str()
                   << "' misnamed. Types should be NamedLikeThis.";

            GlobalViolationManager.AddInfractedName(Name.str());
            GlobalViolationManager.AddViolation(
                    new NamingViolation(File, LineNo, ErrMsg.str()));
        }
    }
}

}  // namespace naming
}  // namespace checks
}  // namespace nett