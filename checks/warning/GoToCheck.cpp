/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "GoToCheck.hpp"

#include "../../violations/ViolationManager.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace warning {

void GoToChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::GotoStmt>("gotoStmt")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getGotoLoc();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        std::stringstream ErrMsg;
        ErrMsg << "'goto' is banned and should not be used.";
        GlobalViolationManager.AddViolation(
                new WarningViolation(File.str(), LineNo, ErrMsg.str()));
    }
}

}  // namespace warning
}  // namespace checks
}  // namespace nett