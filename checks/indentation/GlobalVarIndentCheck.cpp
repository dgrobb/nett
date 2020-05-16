/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "GlobalVarIndentCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "IndentCheck.hpp"

#include "clang/Lex/Lexer.h"

#include <sstream>
#include <unordered_set>

using namespace clang::ast_matchers;

namespace nett {
namespace checks {
namespace indentation {

void GlobalVarIndentChecker::run(const MatchFinder::MatchResult& Result) {

    if (const auto* Node =
                    Result.Nodes.getNodeAs<clang::VarDecl>("globalVar")) {

        if (Node->getType()->getAs<clang::RecordType>() ||
                Node->getType()->getAs<clang::EnumType>()) {
            // Structs and enums have different styling rules
            return;
        }

        // We need to check the indentation of the start of the
        // variable declaration
        CheckStatementIndentation(Node->getBeginLoc(), Result.Context, 0);

        // Then we check the continuation indent
        CheckSourceRangeContinuationIndent(
                Node->getBeginLoc(), Node->getEndLoc(), Result.Context, 0, 0);
    }
}

}  // namespace indentation
}  // namespace checks
}  // namespace nett