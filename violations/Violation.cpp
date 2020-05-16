/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "Violation.hpp"

#include "clang/Frontend/FrontendActions.h"

#include <functional>
#include <sstream>

namespace nett {

// Violation ------------------------------------------------------------------

Violation::Violation(
        std::string SourceFilePath, uint LineNo, std::string Message) {
    this->SourceFilePath = std::move(SourceFilePath);
    this->LineNo = LineNo;
    this->Message = std::move(Message);
}

std::string Violation::GetSourceFile(void) const {
    return Violation::SourceFilePath;
}

std::string Violation::GetMessage(void) const {
    return Violation::Message;
}

uint Violation::GetLineNumber(void) const {
    return Violation::LineNo;
}

std::string Violation::ToTerminalString(void) {
    return std::string("");
}

std::string Violation::ToFileString(void) {
    return std::string("");
}

ViolationType Violation::Type(void) {
    return ViolationType::UNSPEC;
}

bool Violation::operator==(const Violation& Other) const {
    // Two violations are equal if they have the same type,
    // are on the same line in the same file, and have the
    // same message
    if (Violation::LineNo != Other.LineNo) {
        return false;
    }
    if (Violation::SourceFilePath.compare(Other.SourceFilePath) != 0) {
        return false;
    }
    if (Violation::Message.compare(Other.Message) != 0) {
        return false;
    }
    return true;
}

// NamingViolation ------------------------------------------------------------
NamingViolation::NamingViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string NamingViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [NAMING] "
            << Violation::Message;

    return Message.str();
}

std::string NamingViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[NAMING] " << Violation::Message;

    return Message.str();
}

ViolationType NamingViolation::Type(void) {
    return ViolationType::NAMING;
}

// CommentsViolation ----------------------------------------------------------
CommentsViolation::CommentsViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string CommentsViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [COMMENTS] "
            << Violation::Message;

    return Message.str();
}

std::string CommentsViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[COMMENTS] " << Violation::Message;

    return Message.str();
}

ViolationType CommentsViolation::Type(void) {
    return ViolationType::COMMENTS;
}

// BracesViolation ------------------------------------------------------------
BracesViolation::BracesViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string BracesViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [BRACES] "
            << Violation::Message;

    return Message.str();
}

std::string BracesViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[BRACES] " << Violation::Message;

    return Message.str();
}

ViolationType BracesViolation::Type(void) {
    return ViolationType::BRACES;
}

// WhitespaceViolation --------------------------------------------------------
WhitespaceViolation::WhitespaceViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string WhitespaceViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [WHITESPACE] "
            << Violation::Message;

    return Message.str();
}

std::string WhitespaceViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[WHITESPACE] " << Violation::Message;

    return Message.str();
}

ViolationType WhitespaceViolation::Type(void) {
    return ViolationType::WHITESPACE;
}

// IndentationViolation -------------------------------------------------------
IndentationViolation::IndentationViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string IndentationViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [INDENTATION] "
            << Violation::Message;

    return Message.str();
}

std::string IndentationViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[INDENTATION] " << Violation::Message;

    return Message.str();
}

ViolationType IndentationViolation::Type(void) {
    return ViolationType::INDENTATION;
}

// LineLengthViolation -------------------------------------------------------
LineLengthViolation::LineLengthViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string LineLengthViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [LINE_LENGTH] "
            << Violation::Message;

    return Message.str();
}

std::string LineLengthViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[LINE_LENGTH] " << Violation::Message;

    return Message.str();
}

ViolationType LineLengthViolation::Type(void) {
    return ViolationType::LINE_LENGTH;
}

// OverallViolation -----------------------------------------------------------
OverallViolation::OverallViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string OverallViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [OVERALL] "
            << Violation::Message;

    return Message.str();
}

std::string OverallViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[OVERALL] " << Violation::Message;

    return Message.str();
}

ViolationType OverallViolation::Type(void) {
    return ViolationType::OVERALL;
}

// WarningViolation -----------------------------------------------------------
WarningViolation::WarningViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string WarningViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [WARNING] "
            << Violation::Message;

    return Message.str();
}

std::string WarningViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[WARNING] " << Violation::Message;

    return Message.str();
}

ViolationType WarningViolation::Type(void) {
    return ViolationType::WARNING;
}

// NoteViolation -----------------------------------------------------------
NoteViolation::NoteViolation(
        std::string SourceFilePath, uint LineNo, std::string Message)
    : Violation(SourceFilePath, LineNo, Message) {
}

std::string NoteViolation::ToTerminalString(void) {

    std::ostringstream Message;
    Message << "Line " << Violation::LineNo << ": [NOTE] "
            << Violation::Message;

    return Message.str();
}

std::string NoteViolation::ToFileString(void) {

    std::ostringstream Message;
    Message << "[NOTE] " << Violation::Message;

    return Message.str();
}

ViolationType NoteViolation::Type(void) {
    return ViolationType::NOTE;
}

}  // namespace nett