/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "VarNameCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "Hungarian.hpp"
#include "NamingStyles.hpp"

#include <sstream>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace naming {

void VarNameChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        const auto& SM = *Result.SourceManager;
        const auto& Loc = Node->getLocation();

        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }

        auto Name = Node->getName();
        auto File = SM.getFilename(Loc);
        auto LineNo = SM.getExpansionLineNumber(Loc);

        // If the variable name is empty (''), we have a function pointer as
        // a parameter/variable and we ignore it
        if (Name.compare("") == 0) {
            return;
        }

        // Variables need to be lowerCamelCase
        if (!nett::naming::IdentifierFollowsNamingStyle(
                    Name, nett::naming::LOWER_CAMEL_CASE)) {
            std::stringstream ErrMsg;

            if (!GlobalViolationManager.NameHasBeenInfracted(Name)) {
                ErrMsg << "'" << Name.str()
                       << "' misnamed. Variables should be namedLikeThis.";

                GlobalViolationManager.AddInfractedName(Name.str());
                GlobalViolationManager.AddViolation(
                        new NamingViolation(File.str(), LineNo, ErrMsg.str()));
            } else if (GlobalViolationManager.WillOutputToFile()) {
                ErrMsg << "'" << Name.str()
                       << "' misnamed but has already been infracted.";
                GlobalViolationManager.AddViolation(
                        new NoteViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }

        // Variable names should not follow Hungarian notation
        if (nett::naming::IsHungarianVarName(Name, Node->getType())) {
            std::stringstream ErrMsg;

            if (!GlobalViolationManager.NameHasBeenInfracted(Name)) {
                ErrMsg << "'" << Name.str()
                       << "' misnamed. Hungarian notation is not to be used.";

                GlobalViolationManager.AddInfractedName(Name.str());
                GlobalViolationManager.AddViolation(
                        new NamingViolation(File.str(), LineNo, ErrMsg.str()));
            } else if (GlobalViolationManager.WillOutputToFile()) {
                ErrMsg << "'" << Name.str()
                       << "' misnamed but has already been infracted.";
                GlobalViolationManager.AddViolation(
                        new NoteViolation(File.str(), LineNo, ErrMsg.str()));
            }
        }
    }
}

}  // namespace naming
}  // namespace checks
}  // namespace nett