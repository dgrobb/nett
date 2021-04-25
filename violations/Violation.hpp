/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_VIOLATIONS_VIOLATION_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_VIOLATIONS_VIOLATION_HPP

#include <string>

namespace nett {

// A category of style violation
enum ViolationType {
    NAMING,
    COMMENTS,
    BRACES,
    WHITESPACE,
    INDENTATION,
    LINE_LENGTH,
    OVERALL,
    WARNING,
    NOTE,
    UNSPEC
};

// Represents a style violation within a source file.
class Violation {
    public:
    Violation(std::string SourceFilePath, uint LineNo, std::string Message);

    // Returns the full file path of the file in which
    // this violation has occurred.
    std::string GetSourceFile(void) const;

    // Returns the line number in the file on which this
    // violation occured.
    uint GetLineNumber(void) const;

    // Returns the message string for this violation
    std::string GetMessage(void) const;

    // Returns a string representation of this violation
    // for terminal output purposes.
    virtual std::string ToTerminalString(void);

    // Returns a string representation of this violation
    // for file output purposes.
    virtual std::string ToFileString(void);

    // Returns the type of this violation (NAMING, COMMENTS etc.)
    virtual ViolationType Type(void);

    // We need to override the equals method
    bool operator==(const Violation& Other) const;

    protected:
    std::string SourceFilePath;
    uint LineNo;
    std::string Message;
};

class NamingViolation : public Violation {
    public:
    NamingViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class CommentsViolation : public Violation {
    public:
    CommentsViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class BracesViolation : public Violation {
    public:
    BracesViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class WhitespaceViolation : public Violation {
    public:
    WhitespaceViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class IndentationViolation : public Violation {
    public:
    IndentationViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class LineLengthViolation : public Violation {
    public:
    LineLengthViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class OverallViolation : public Violation {
    public:
    OverallViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class WarningViolation : public Violation {
    public:
    WarningViolation(
            std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

class NoteViolation : public Violation {
    public:
    NoteViolation(std::string SourceFilePath, uint LineNo, std::string Message);
    std::string ToTerminalString(void) override;
    std::string ToFileString(void) override;
    ViolationType Type(void) override;
};

}  // namespace nett

#endif