/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FuncNameCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "NamingStyles.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace naming {

void FuncNameChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto Name = Node->getName();
        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        // We only want to match function definitions in order to get consistent
        // error reporting.
        if (!Node->isThisDeclarationADefinition()) {
            return;
        }

        // Function names need to be lower_snake_case
        if (!nett::naming::IdentifierFollowsNamingStyle(
                    Name, nett::naming::LOWER_SNAKE_CASE)) {
            std::stringstream ErrMsg;

            // We don't need to check if a function name has already been
            // infracted since function names must be unique within a C program.
            ErrMsg << "'" << Name.str()
                   << "' misnamed. Functions should be named_like_this.";

            GlobalViolationManager.AddInfractedName(Name.str());
            GlobalViolationManager.AddViolation(
                    new NamingViolation(File.str(), LineNo, ErrMsg.str()));
        }
    }
}

}  // namespace naming
}  // namespace checks
}  // namespace nett