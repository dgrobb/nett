/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "WhitespaceCheck.hpp"

#include "../../violations/ViolationManager.hpp"
#include "../utils/Tokens.hpp"
#include "PointerStyles.hpp"

#include "clang/Lex/Lexer.h"

#include <sstream>

namespace nett {
namespace checks {
namespace whitespace {

void GenerateWhitespaceViolation(clang::SourceLocation ErrLoc,
        clang::SourceManager& SM, std::string TokenString, unsigned ColNo,
        unsigned ExpectedSpaces, unsigned ActualSpaces) {

    auto File = SM.getFilename(ErrLoc);
    auto LineNo = SM.getSpellingLineNumber(ErrLoc);

    std::string SpaceString;
    if (ExpectedSpaces == 1) {
        SpaceString = "space";
    } else {
        SpaceString = "spaces";
    }

    std::stringstream ErrMsg;
    ErrMsg << "'" << TokenString << "' "
           << "at position " << ColNo << ", "
           << "expected " << ExpectedSpaces << " " << SpaceString << ", "
           << "found " << ActualSpaces;

    GlobalViolationManager.AddViolation(
            new WhitespaceViolation(File.str(), LineNo, ErrMsg.str()));
}

void CheckLocationWhitespace(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, unsigned ExpectedSpaces,
        clang::SourceManager& SM, clang::LangOptions LangOpts) {

    // If the locations are not on the same line, then
    // we cannot check the whitespace correctly
    if (SM.getSpellingLineNumber(StartLoc) !=
            SM.getSpellingLineNumber(EndLoc)) {
        return;
    }

    auto StartColNo = SM.getSpellingColumnNumber(StartLoc);
    auto EndColNo = SM.getSpellingColumnNumber(EndLoc);

    if (EndColNo - StartColNo != ExpectedSpaces + 1) {
        GenerateWhitespaceViolation(EndLoc, SM,
                utils::GetTokenSourceString(EndLoc, SM, LangOpts), EndColNo,
                ExpectedSpaces, EndColNo - StartColNo - 1);
    }
}

clang::SourceLocation GetPreviousNonWhitespaceLoc(
        clang::SourceLocation StartLoc, clang::SourceManager& SM) {

    clang::SourceLocation Result = StartLoc;
    while (utils::isWhitespace(*SM.getCharacterData(Result))) {
        Result = Result.getLocWithOffset(-1);
    }
    return Result;
}

clang::SourceLocation GetNextNonWhitespaceLoc(
        clang::SourceLocation StartLoc, clang::SourceManager& SM) {

    clang::SourceLocation Result = StartLoc;
    while (utils::isWhitespace(*SM.getCharacterData(Result))) {
        Result = Result.getLocWithOffset(1);
    }
    return Result;
}

void CheckParenWhitespace(clang::SourceLocation LParenLoc,
        clang::SourceLocation RParenLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    clang::SourceLocation CurrentLoc;
    auto File = SM.getFilename(LParenLoc);
    auto LParenLineNo = SM.getSpellingLineNumber(LParenLoc);
    auto RParenLineNo = SM.getSpellingLineNumber(RParenLoc);

    // There should be no spaces between the opening parenthesis
    // and the start of the condition -> e.g. (cond...
    // Also, the start of the condition should be on the same line
    // as the opening parenthesis
    CurrentLoc = LParenLoc.getLocWithOffset(1);
    while (utils::isWhitespace(*SM.getCharacterData(CurrentLoc))) {
        CurrentLoc = CurrentLoc.getLocWithOffset(1);
    }

    if (SM.getSpellingLineNumber(CurrentLoc) == LParenLineNo) {
        CheckLocationWhitespace(LParenLoc, CurrentLoc, 0, SM, LangOpts);
    }

    // There should be no spaces between the closing parenthesis
    // and the end of the condition -> e.g. ...cond)
    // Also, the end of the condition should be on the same line
    // as the closing parenthesis
    CurrentLoc = RParenLoc.getLocWithOffset(-1);
    while (utils::isWhitespace(*SM.getCharacterData(CurrentLoc))) {
        CurrentLoc = CurrentLoc.getLocWithOffset(-1);
    }

    if (SM.getSpellingLineNumber(CurrentLoc) != RParenLineNo) {
        std::stringstream ErrMsg;
        ErrMsg << "Closing parentheses should look like: ...X)";
        GlobalViolationManager.AddViolation(new WhitespaceViolation(
                File.str(), RParenLineNo, ErrMsg.str()));
    } else {
        if (LParenLoc != CurrentLoc) {
            CheckLocationWhitespace(CurrentLoc, RParenLoc, 0, SM, LangOpts);
        }
    }
}

void CheckSourceRangeCommaWhitespace(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    auto SourceRange = clang::SourceRange(StartLoc, EndLoc);
    auto CharRange = clang::Lexer::getAsCharRange(SourceRange, SM, LangOpts);
    auto SourceText = clang::Lexer::getSourceText(CharRange, SM, LangOpts);

    auto Comma = std::string(",");
    auto Offset = SourceText.find(Comma);

    while (Offset != llvm::StringRef::npos) {
        auto NextCommaLocation = StartLoc.getLocWithOffset(Offset);
        auto PrevTokenLoc = GetPreviousNonWhitespaceLoc(
                NextCommaLocation.getLocWithOffset(-1), SM);
        auto NextTokenLoc = GetNextNonWhitespaceLoc(
                NextCommaLocation.getLocWithOffset(1), SM);

        // Check that we aren't inside a string
        auto CurrentTokenLoc = clang::Lexer::GetBeginningOfToken(
                NextCommaLocation, SM, LangOpts);
        auto CurrentToken = clang::Lexer::findNextToken(
                CurrentTokenLoc.getLocWithOffset(-1), SM, LangOpts);

        if (CurrentToken->getKind() != clang::tok::comma) {
            Offset = SourceText.find(",", Offset + Comma.size());
            continue;
        }

        // We also need to skip comments
        clang::Token Temp;
        clang::Lexer::getRawToken(NextTokenLoc, Temp, SM, LangOpts);

        if (SM.getSpellingLineNumber(PrevTokenLoc) ==
                SM.getSpellingLineNumber(NextCommaLocation)) {
            // There should be no space before the comma
            CheckLocationWhitespace(
                    PrevTokenLoc, NextCommaLocation, 0, SM, LangOpts);
        }
        if (Temp.getKind() != clang::tok::comment &&
                SM.getSpellingLineNumber(NextTokenLoc) ==
                        SM.getSpellingLineNumber(NextCommaLocation)) {
            // There should be one space after the comma
            CheckLocationWhitespace(
                    NextCommaLocation, NextTokenLoc, 1, SM, LangOpts);
        }

        Offset = SourceText.find(",", Offset + Comma.size());
    }
}

int CountLocationWhitespace(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM) {

    // If the locations are not on the same line, then
    // we cannot check the whitespace correctly
    if (SM.getExpansionLineNumber(StartLoc) !=
            SM.getExpansionLineNumber(EndLoc)) {
        return -1;
    }

    auto StartColNo = SM.getExpansionColumnNumber(StartLoc);
    auto EndColNo = SM.getExpansionColumnNumber(EndLoc);

    return EndColNo - StartColNo - 1;
}

void CheckPointerAlignment(clang::SourceLocation PtrLoc,
        enum PointerStyle ExpectedStyle, clang::SourceManager& SM) {

    auto File = SM.getFilename(PtrLoc);

    if (PtrStyle != ExpectedStyle) {
        std::stringstream ErrMsg;
        ErrMsg << "Pointers should consistently be a* b or a *b, not a mix.";
        GlobalViolationManager.AddViolation(new WhitespaceViolation(
                File.str(), SM.getExpansionLineNumber(PtrLoc), ErrMsg.str()));
    }
}

// Checks if the given pointer token is within brackets '[]'
// If so, it is not a pointer, but a multiplication sign. So we return true.
// Otherwise, if no brackets are found within the given min and
// max source locations, returns false.
bool PointerInBrackets(llvm::Optional<clang::Token> Token,
        clang::SourceManager& SM, clang::LangOptions LangOpts,
        clang::SourceLocation RangeMinLoc, clang::SourceLocation RangeMaxLoc) {

    auto PointerLoc = SM.getFileLoc(Token->getLocation());
    auto LHS = PointerLoc.getLocWithOffset(-1);
    auto RHS = PointerLoc.getLocWithOffset(1);

    bool FoundOpeningBracket = false;
    bool FoundClosingBracket = false;

    while (LHS >= RangeMinLoc) {
        if (*SM.getCharacterData(LHS) == '[') {
            FoundOpeningBracket = true;
            break;
        }
        LHS = LHS.getLocWithOffset(-1);
    }

    while (RHS <= RangeMaxLoc) {
        if (*SM.getCharacterData(RHS) == ']') {
            FoundClosingBracket = true;
            break;
        }
        RHS = RHS.getLocWithOffset(1);
    }

    return FoundOpeningBracket && FoundClosingBracket;
}

void CheckPointerSpacing(llvm::Optional<clang::Token> CurrentToken,
        llvm::Optional<clang::Token> NextToken, clang::SourceManager& SM,
        clang::LangOptions LangOpts, clang::SourceLocation RangeMinLoc,
        clang::SourceLocation RangeMaxLoc) {

    auto CurrentTokenData = *SM.getCharacterData(CurrentToken->getLocation());
    auto NextTokenData = *SM.getCharacterData(NextToken->getLocation());
    auto File = SM.getFilename(CurrentToken->getLocation());

    if (CurrentTokenData == '*' && NextTokenData == '*') {
        // We have two pointers next to one another. (i.e. **)
        // They should be on the same line and have no spaces separating them
        auto LeftPtrLoc = CurrentToken->getLocation();
        auto RightPtrLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(LeftPtrLoc) !=
                SM.getExpansionLineNumber(RightPtrLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Pointers should not be split across lines.";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(LeftPtrLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(LeftPtrLoc, RightPtrLoc, 0, SM, LangOpts);
        }
    }
    if (CurrentTokenData == '*' && NextTokenData != '*') {
        // We have a pointer followed by another token. (e.g. *x)

        // Check whether the next token is valid for a pointer
        if (PointerInBrackets(
                    CurrentToken, SM, LangOpts, RangeMinLoc, RangeMaxLoc)) {
            return;
        }

        auto PtrLoc = CurrentToken->getLocation();
        auto TokenLoc = NextToken->getLocation();

        if (SM.getExpansionLineNumber(PtrLoc) !=
                SM.getExpansionLineNumber(TokenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Pointers should look like this: *X";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(PtrLoc), ErrMsg.str()));
            return;
        }

        // We have to consider the following cases:
        //      -> it is a function pointer: (*name)(...
        //      -> it is a pointer followed by a comma: int[*,]
        //      -> it is a right-aligned pointer: int [*token]
        //      -> it is a left-aligned pointer: int[* token]

        // Function pointer case
        auto RParenToken = clang::Lexer::findNextToken(
                NextToken->getLocation(), SM, LangOpts);
        auto LParenToken = clang::Lexer::findNextToken(
                RParenToken->getLocation(), SM, LangOpts);
        if (*SM.getCharacterData(RParenToken->getLocation()) == ')' &&
                *SM.getCharacterData(LParenToken->getLocation()) == '(') {
            // There should be no space between the pointer and the variable
            // name
            CheckLocationWhitespace(PtrLoc, TokenLoc, 0, SM, LangOpts);
            return;
        }

        // Comma case (this is already checked elsewhere)
        if (NextTokenData == ',') {
            return;
        }

        // Pointer alignment case
        auto PrevTokenStart = clang::Lexer::GetBeginningOfToken(
                GetPreviousNonWhitespaceLoc(PtrLoc.getLocWithOffset(-1), SM),
                SM, LangOpts);
        auto PrevToken = clang::Lexer::findNextToken(
                PrevTokenStart.getLocWithOffset(-1), SM, LangOpts);
        auto PrevTokenLoc = PrevToken->getLocation().getLocWithOffset(
                PrevToken->getLength() - 1);

        // We don't check alignment if all three parts are not on the same line
        if (SM.getExpansionLineNumber(PrevTokenLoc) !=
                SM.getExpansionLineNumber(PtrLoc)) {
            return;
        }

        // Comma case: , *f <- we want the pointer to be right aligned
        if (*SM.getCharacterData(PrevTokenLoc) == ',') {
            CheckLocationWhitespace(PtrLoc, TokenLoc, 0, SM, LangOpts);
            return;
        }

        auto PrevToPtrSpaceCount =
                CountLocationWhitespace(PrevTokenLoc, PtrLoc, SM);
        auto PtrToNextSpaceCount =
                CountLocationWhitespace(PtrLoc, TokenLoc, SM);

        if (PrevToPtrSpaceCount == 0 && PtrToNextSpaceCount != 0) {
            // We have: token* token (left aligned)
            SetPointerStyle(PointerStyle::PTR_LEFT);
            CheckPointerAlignment(PtrLoc, PointerStyle::PTR_LEFT, SM);
        } else if (PrevToPtrSpaceCount != 0 && PtrToNextSpaceCount == 0) {
            // We have: token *token (right aligned)
            if (*SM.getCharacterData(PrevTokenLoc) == '(') {
                // We have: ( *token, so we ignore it since parentheses are
                // checked elsewhere
                return;
            }
            SetPointerStyle(PointerStyle::PTR_RIGHT);
            CheckPointerAlignment(PtrLoc, PointerStyle::PTR_RIGHT, SM);
        } else if (PrevToPtrSpaceCount == 0 && PtrToNextSpaceCount == 0) {
            // We have: token*token
            if (*SM.getCharacterData(PrevTokenLoc) == '*') {
                // We have: **token (right alignment)
                SetPointerStyle(PointerStyle::PTR_RIGHT);
                CheckPointerAlignment(PtrLoc, PointerStyle::PTR_RIGHT, SM);
                return;
            } else if (*SM.getCharacterData(PrevTokenLoc) == '(') {
                // We have: (*token, so we ignore it
                return;
            } else if (*SM.getCharacterData(TokenLoc) == ')') {
                // We have: token*), so we ignore it
                return;
            }

            // We have too little space.
            std::stringstream ErrMsg;
            ErrMsg << "Pointers should be a* b or a *b, not a*b.";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(PtrLoc), ErrMsg.str()));
        } else if (PrevToPtrSpaceCount != 0 && PtrToNextSpaceCount != 0) {
            // We have: token * token (too much space)
            std::stringstream ErrMsg;
            ErrMsg << "Pointers should be a* b or a *b, not a * b.";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(PtrLoc), ErrMsg.str()));
        }
    }
    if (CurrentTokenData != '*' && NextTokenData == '*') {
        // We have a token followed by a pointer. (e.g. int*)

        // Check whether the current token is valid for a pointer
        if (PointerInBrackets(
                    NextToken, SM, LangOpts, RangeMinLoc, RangeMaxLoc)) {
            return;
        }

        // They should be on the same line
        auto TokenLoc = CurrentToken->getLocation().getLocWithOffset(
                CurrentToken->getLength() - 1);
        auto PtrLoc = NextToken->getLocation();

        if (SM.getExpansionLineNumber(PtrLoc) !=
                SM.getExpansionLineNumber(TokenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Pointers should look like this: X*";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(PtrLoc), ErrMsg.str()));
            return;
        }

        // We have to consider the following cases:
        //      -> it is pointer within parentheses
        //      -> the pointer is after a comma: , *f
        //      -> it is a right-aligned pointer: [int *]token
        //      -> it is a left-aligned pointer [int*] token

        // Parenthesis case: ignore since this is checked elsewhere
        if (*SM.getCharacterData(TokenLoc) == '(') {
            return;
        }

        // Pointer alignment case
        auto NextToken = clang::Lexer::findNextToken(PtrLoc, SM, LangOpts);
        auto NextTokenLoc = NextToken->getLocation();

        // We don't check alignment if all three parts are not on the same line
        if (SM.getExpansionLineNumber(NextTokenLoc) !=
                SM.getExpansionLineNumber(PtrLoc)) {
            return;
        }

        // Comma case: , *f <- we want the pointer to be right aligned
        if (*SM.getCharacterData(TokenLoc) == ',') {
            CheckLocationWhitespace(PtrLoc, NextTokenLoc, 0, SM, LangOpts);
            return;
        }

        auto PrevToPtrSpaceCount =
                CountLocationWhitespace(TokenLoc, PtrLoc, SM);
        auto PtrToNextSpaceCount =
                CountLocationWhitespace(PtrLoc, NextTokenLoc, SM);

        if (PrevToPtrSpaceCount == 0 && PtrToNextSpaceCount != 0) {
            // We have: token* nextToken (left aligned)
            SetPointerStyle(PointerStyle::PTR_LEFT);
            CheckPointerAlignment(PtrLoc, PointerStyle::PTR_LEFT, SM);
        } else if (PrevToPtrSpaceCount != 0 && PtrToNextSpaceCount == 0) {
            // We have: token *nextToken (right aligned)
            SetPointerStyle(PointerStyle::PTR_RIGHT);
            CheckPointerAlignment(PtrLoc, PointerStyle::PTR_RIGHT, SM);
        } else if (PrevToPtrSpaceCount == 0 && PtrToNextSpaceCount == 0) {
            // We have: token*nextToken
            if (*SM.getCharacterData(NextTokenLoc) == '*') {
                // We have: token** (left alignment)
                SetPointerStyle(PointerStyle::PTR_LEFT);
                CheckPointerAlignment(PtrLoc, PointerStyle::PTR_LEFT, SM);
                return;
            } else if (*SM.getCharacterData(NextTokenLoc) == ')') {
                // We have: token*), so we ignore it
                return;
            } else if (*SM.getCharacterData(TokenLoc) == '(') {
                // We have: (*token, so we ignore it
                return;
            } else if (*SM.getCharacterData(NextTokenLoc) == ',') {
                // We have: "token*,", so we ignore it
                return;
            }

            // Otherwise we have too little space
            std::stringstream ErrMsg;
            ErrMsg << "Pointers should be a* b or a *b, not a*b.";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(PtrLoc), ErrMsg.str()));
        } else if (PrevToPtrSpaceCount != 0 && PtrToNextSpaceCount != 0) {
            // We have: token * nextToken (too much space)
            std::stringstream ErrMsg;
            ErrMsg << "Pointers should be a* b or a *b, not a * b.";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(PtrLoc), ErrMsg.str()));
        }
    }
}

void CheckSquareBracketSpacing(llvm::Optional<clang::Token> CurrentToken,
        llvm::Optional<clang::Token> NextToken, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    auto CurrentTokenData = *SM.getCharacterData(CurrentToken->getLocation());
    auto NextTokenData = *SM.getCharacterData(NextToken->getLocation());
    auto File = SM.getFilename(CurrentToken->getLocation());

    if (CurrentTokenData == '[' && NextTokenData == ']') {
        // We have two brackets next to one another. (e.g. [])
        // They should be on the same line with no space separating them
        auto LeftBracketLoc = CurrentToken->getLocation();
        auto RightBracketLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(LeftBracketLoc) !=
                SM.getExpansionLineNumber(RightBracketLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Empty brackets should look like: []";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(LeftBracketLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(
                    LeftBracketLoc, RightBracketLoc, 0, SM, LangOpts);
        }
    }
    if (CurrentTokenData == '[' && NextTokenData != ']') {
        // We have an open bracket followed by some token. (e.g. [x...)
        // They should be on the same line with no space separating them
        auto LeftBracketLoc = CurrentToken->getLocation();
        auto TokenLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(LeftBracketLoc) !=
                SM.getExpansionLineNumber(TokenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Opening brackets should look like: [X...";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(LeftBracketLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(LeftBracketLoc, TokenLoc, 0, SM, LangOpts);
        }
    }
    if (CurrentTokenData != '[' && NextTokenData == ']') {
        // We have a token followed by a bracket. (e.g. ...x])
        // They should be on the same line with no space separating them
        auto TokenLoc = CurrentToken->getLocation().getLocWithOffset(
                CurrentToken->getLength() - 1);
        auto RightBracketLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(TokenLoc) !=
                SM.getExpansionLineNumber(RightBracketLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Closing brackets should look like: ...X]";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(RightBracketLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(TokenLoc, RightBracketLoc, 0, SM, LangOpts);
        }
    }
    if (NextTokenData == '[') {
        // We have a token followed by a bracket. (e.g. foo[...)
        // The two should be on the same line with no space separating them
        auto TokenLoc = CurrentToken->getLocation().getLocWithOffset(
                CurrentToken->getLength() - 1);
        auto LeftBracketLoc = NextToken->getLocation();

        if (SM.getExpansionLineNumber(TokenLoc) !=
                SM.getExpansionLineNumber(LeftBracketLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Opening brackets should look like: ...X[";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(LeftBracketLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(TokenLoc, LeftBracketLoc, 0, SM, LangOpts);
        }
    }
}

void CheckParenthesisSpacing(llvm::Optional<clang::Token> CurrentToken,
        llvm::Optional<clang::Token> NextToken, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    auto CurrentTokenData = *SM.getCharacterData(CurrentToken->getLocation());
    auto NextTokenData = *SM.getCharacterData(NextToken->getLocation());
    auto File = SM.getFilename(CurrentToken->getLocation());

    if (CurrentTokenData == '(') {
        // There should be no spaces between an opening parenthesis and the next
        // token. The two should also be on the same line
        auto ParenLoc = CurrentToken->getLocation();
        auto TokenLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(ParenLoc) ==
                SM.getExpansionLineNumber(TokenLoc)) {
            CheckLocationWhitespace(ParenLoc, TokenLoc, 0, SM, LangOpts);
        }
    }
    if (NextTokenData == ')') {
        // There should be no spaces between a token and a closing parenthesis
        // The two should also be on the same line
        auto TokenLoc = CurrentToken->getLocation().getLocWithOffset(
                CurrentToken->getLength() - 1);
        auto ParenLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(TokenLoc) !=
                SM.getExpansionLineNumber(ParenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Closing parentheses should look like: ...X)";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(ParenLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(TokenLoc, ParenLoc, 0, SM, LangOpts);
        }
    }
    if (CurrentTokenData == ')' && NextTokenData == '(') {
        // We have a function pointer parenthesis pair: ...)(...
        // The two should be on the same line and should have no spaces between
        // them
        auto RParenLoc = CurrentToken->getLocation();
        auto LParenLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(RParenLoc) !=
                SM.getExpansionLineNumber(LParenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Function pointer parentheses should look like: ...)(...";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(LParenLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(RParenLoc, LParenLoc, 0, SM, LangOpts);
        }
    }
}

void CheckCurlyBraceSpacing(llvm::Optional<clang::Token> CurrentToken,
        llvm::Optional<clang::Token> NextToken, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    auto CurrentTokenData = *SM.getCharacterData(CurrentToken->getLocation());
    auto NextTokenData = *SM.getCharacterData(NextToken->getLocation());
    auto File = SM.getFilename(CurrentToken->getLocation());

    if (CurrentTokenData == '{' && NextTokenData == '}') {
        // We have two brackets next to one another. (e.g. {})
        // They should be on the same line with no space separating them
        auto LeftBracketLoc = CurrentToken->getLocation();
        auto RightBracketLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(LeftBracketLoc) !=
                SM.getExpansionLineNumber(RightBracketLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Empty curly braces should look like: {}";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(LeftBracketLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(
                    LeftBracketLoc, RightBracketLoc, 0, SM, LangOpts);
        }
    }
    if (CurrentTokenData == '{' && NextTokenData != '}') {
        // We have an open bracket followed by some token. (e.g. {x...)
        // They should be on the same line with no space separating them
        auto LeftBracketLoc = CurrentToken->getLocation();
        auto TokenLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(LeftBracketLoc) ==
                SM.getExpansionLineNumber(TokenLoc)) {
            CheckLocationWhitespace(LeftBracketLoc, TokenLoc, 0, SM, LangOpts);
        }
    }
    if (CurrentTokenData != '{' && NextTokenData == '}') {
        // We have a token followed by a bracket. (e.g. ...x})
        // They should be on the same line with no space separating them
        auto TokenLoc = CurrentToken->getLocation().getLocWithOffset(
                CurrentToken->getLength() - 1);
        auto RightBracketLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(TokenLoc) ==
                SM.getExpansionLineNumber(RightBracketLoc)) {
            CheckLocationWhitespace(TokenLoc, RightBracketLoc, 0, SM, LangOpts);
        }
    }
    if (NextTokenData == '{') {
        // We have a token followed by a bracket. (e.g. foo {...)
        // The two should be on the same line with a single space separating
        // them
        auto TokenLoc = CurrentToken->getLocation().getLocWithOffset(
                CurrentToken->getLength() - 1);
        auto LeftBracketLoc = NextToken->getLocation();

        if (SM.getExpansionLineNumber(TokenLoc) !=
                SM.getExpansionLineNumber(LeftBracketLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Opening braces should look like: ...X {";
            GlobalViolationManager.AddViolation(new WhitespaceViolation(
                    File.str(), SM.getExpansionLineNumber(LeftBracketLoc),
                    ErrMsg.str()));
        } else {
            CheckLocationWhitespace(TokenLoc, LeftBracketLoc, 1, SM, LangOpts);
        }
    }
}

void CheckCommaSpacing(llvm::Optional<clang::Token> CurrentToken,
        llvm::Optional<clang::Token> NextToken, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    auto CurrentTokenData = *SM.getCharacterData(CurrentToken->getLocation());
    auto NextTokenData = *SM.getCharacterData(NextToken->getLocation());
    auto File = SM.getFilename(CurrentToken->getLocation());

    if (CurrentTokenData == ',' && NextTokenData != ',') {
        // We have a situation like: , X
        // If the two are on the same line, then there should be a single space
        auto CommaLoc = CurrentToken->getLocation();
        auto TokenLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(CommaLoc) ==
                SM.getExpansionLineNumber(TokenLoc)) {
            CheckLocationWhitespace(CommaLoc, TokenLoc, 1, SM, LangOpts);
        }
    }

    if (CurrentTokenData != ',' && NextTokenData == ',') {
        // We have a situation like: X,
        // The two should be on the same line, and should have no spaces
        auto TokenLoc = CurrentToken->getLocation().getLocWithOffset(
                CurrentToken->getLength() - 1);
        auto CommaLoc = NextToken->getLocation();
        if (SM.getExpansionLineNumber(CommaLoc) !=
                SM.getExpansionLineNumber(TokenLoc)) {
            std::stringstream ErrMsg;
            ErrMsg << "Commas should look like: X,";
            GlobalViolationManager.AddViolation(
                    new WhitespaceViolation(File.str(),
                            SM.getExpansionLineNumber(CommaLoc), ErrMsg.str()));
        } else {
            CheckLocationWhitespace(TokenLoc, CommaLoc, 0, SM, LangOpts);
        }
    }
}

void CheckGeneralSpacing(llvm::Optional<clang::Token> CurrentToken,
        llvm::Optional<clang::Token> NextToken, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    auto CurrentTokenData = CurrentToken->getKind();
    auto NextTokenData = NextToken->getKind();

    auto TokensToIgnore = {clang::tok::l_square, clang::tok::r_square,
            clang::tok::l_paren, clang::tok::r_paren, clang::tok::l_brace,
            clang::tok::r_brace, clang::tok::period, clang::tok::comma,
            clang::tok::semi, clang::tok::star, clang::tok::amp,
            clang::tok::plus, clang::tok::minus, clang::tok::arrow,
            clang::tok::tilde, clang::tok::exclaim, clang::tok::comment,
            clang::tok::minusminus, clang::tok::plusplus, clang::tok::caret,
            clang::tok::colon};

    for (auto Token : TokensToIgnore) {
        if (Token == CurrentTokenData || Token == NextTokenData) {
            return;
        }
    }

    // Here we have two tokens which aren't special cases.
    // They don't need to be on the same line, but if they are
    // there must be a single space separating them
    auto CurrentTokenEndLoc = clang::Lexer::getLocForEndOfToken(
            CurrentToken->getLocation(), 1, SM, LangOpts);
    auto NextTokenStartLoc = NextToken->getLocation();

    if (SM.getExpansionLineNumber(CurrentTokenEndLoc) ==
            SM.getExpansionLineNumber(NextTokenStartLoc)) {
        CheckLocationWhitespace(
                CurrentTokenEndLoc, NextTokenStartLoc, 1, SM, LangOpts);
    }
}

void CheckSourceRangeWhitespaceTokens(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    clang::Token CurrentToken;
    clang::Token NextToken;

    StartLoc = GetNextNonWhitespaceLoc(StartLoc, SM);
    clang::Lexer::getRawToken(StartLoc, CurrentToken, SM, LangOpts);

    do {
        auto CurrEndLoc = CurrentToken.getEndLoc();
        auto NextStartLoc = GetNextNonWhitespaceLoc(CurrEndLoc, SM);
        clang::Lexer::getRawToken(NextStartLoc, NextToken, SM, LangOpts);

        if (NextToken.getLocation() > EndLoc ||
                CurrentToken.getLocation() == NextToken.getLocation()) {
            break;
        }

        auto CurrTok = llvm::Optional<clang::Token>(CurrentToken);
        auto NextTok = llvm::Optional<clang::Token>(NextToken);

        CheckPointerSpacing(CurrTok, NextTok, SM, LangOpts, StartLoc, EndLoc);
        CheckSquareBracketSpacing(CurrTok, NextTok, SM, LangOpts);
        CheckParenthesisSpacing(CurrTok, NextTok, SM, LangOpts);
        CheckCommaSpacing(CurrTok, NextTok, SM, LangOpts);
        CheckGeneralSpacing(CurrTok, NextTok, SM, LangOpts);

        CurrentToken = NextToken;
    } while (NextToken.getLocation() <= EndLoc);
}

void CheckSourceRangeWhitespaceTokensNoPointers(clang::SourceLocation StartLoc,
        clang::SourceLocation EndLoc, clang::SourceManager& SM,
        clang::LangOptions LangOpts) {

    clang::Token CurrentToken;
    clang::Token NextToken;

    StartLoc = GetNextNonWhitespaceLoc(StartLoc, SM);
    clang::Lexer::getRawToken(StartLoc, CurrentToken, SM, LangOpts);

    do {
        auto CurrEndLoc = CurrentToken.getEndLoc();
        auto NextStartLoc = GetNextNonWhitespaceLoc(CurrEndLoc, SM);
        clang::Lexer::getRawToken(NextStartLoc, NextToken, SM, LangOpts);

        if (NextToken.getLocation() > EndLoc ||
                CurrentToken.getLocation() == NextToken.getLocation()) {
            break;
        }

        auto CurrTok = llvm::Optional<clang::Token>(CurrentToken);
        auto NextTok = llvm::Optional<clang::Token>(NextToken);

        CheckSquareBracketSpacing(CurrTok, NextTok, SM, LangOpts);
        CheckParenthesisSpacing(CurrTok, NextTok, SM, LangOpts);
        CheckCommaSpacing(CurrTok, NextTok, SM, LangOpts);
        CheckGeneralSpacing(CurrTok, NextTok, SM, LangOpts);

        CurrentToken = NextToken;
    } while (NextToken.getLocation() <= EndLoc);
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett