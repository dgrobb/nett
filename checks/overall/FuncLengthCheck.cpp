/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "FuncLengthCheck.hpp"

#include "../../violations/ViolationManager.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace overall {

#define MAX_FUNCTION_LENGTH 50

void FuncLengthChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        if (Node->isThisDeclarationADefinition()) {
            if (const clang::Stmt* Body = Node->getBody()) {
                if (SM.isWrittenInSameFile(
                            Body->getBeginLoc(), Body->getEndLoc())) {
                    auto StartLine =
                            SM.getExpansionLineNumber(Body->getBeginLoc());
                    auto EndLine = SM.getExpansionLineNumber(Body->getEndLoc());
                    auto FunctionLength = EndLine - StartLine;

                    if (FunctionLength > MAX_FUNCTION_LENGTH) {
                        std::stringstream ErrMsg;
                        ErrMsg << "Function length of " << FunctionLength
                               << " is over the maximum of "
                               << MAX_FUNCTION_LENGTH << ".";

                        GlobalViolationManager.AddViolation(
                                new OverallViolation(
                                        File.str(), LineNo, ErrMsg.str()));
                    }
                }
            }
        }
    }
}

}  // namespace overall
}  // namespace checks
}  // namespace nett