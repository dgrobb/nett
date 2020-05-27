/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "StructUnionNameCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "NamingStyles.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace naming {

void RecordNameChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::RecordDecl>("recordDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto Name = Node->getName();
        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        if (Name.compare("") == 0) {
            // We have an anonymous struct/union, so we ignore it
            return;
        }

        // Struct/Union names need to be UpperCamelCase
        if (!nett::naming::IdentifierFollowsNamingStyle(
                    Name, nett::naming::UPPER_CAMEL_CASE)) {
            std::stringstream ErrMsg;

            // We only want to log the error at the struct/union definition
            if (Node->isCompleteDefinition()) {
                ErrMsg << "'" << Name.str()
                       << "' misnamed. Types should be NamedLikeThis.";

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