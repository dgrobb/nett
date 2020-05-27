/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FuncDefinitionInHeaderCheck.hpp"

#include "../../violations/ViolationManager.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace overall {

void FuncDefinitionInHeaderChecker::run(
        const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        if (Node->isThisDeclarationADefinition() && File.endswith(".h")) {
            std::stringstream ErrMsg;
            ErrMsg << "Headers should not contain function definitions.";

            GlobalViolationManager.AddViolation(
                    new OverallViolation(File.str(), LineNo, ErrMsg.str()));
        }
    }
}

}  // namespace overall
}  // namespace checks
}  // namespace nett