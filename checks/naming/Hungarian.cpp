/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "Hungarian.hpp"

namespace nett {
namespace naming {

static bool IsHungarianNameFuncPtr(llvm::StringRef VarName);
static bool IsHungarianNameChar(llvm::StringRef VarName);
static bool IsHungarianNamePtr(llvm::StringRef VarName);
static bool IsHungarianNameArray(llvm::StringRef VarName);
static bool IsHungarianNameStruct(llvm::StringRef VarName);
static bool IsHungarianNameEnum(llvm::StringRef VarName);

bool IsHungarianVarName(llvm::StringRef VarName, clang::QualType VarType) {

    llvm::StringRef VarTypeString = VarType.getAsString();
    auto NotFound = llvm::StringRef::npos;

    if (VarType->isFunctionPointerType()) {
        return IsHungarianNameFuncPtr(VarName) || IsHungarianNamePtr(VarName);
    } else if (VarTypeString.find("*") != NotFound) {
        // If the type contains a pointer, it can be one of the following:
        //      -> a pointer to an array
        //      -> simply a pointer to something (int*, char* etc.)
        // We also ignore string checks because often 'str' and 'string'
        // are appropriate names.
        return IsHungarianNameArray(VarName) || IsHungarianNamePtr(VarName);
    } else if (VarTypeString.find("[") != NotFound &&
               VarTypeString.find("]") != NotFound) {
        return IsHungarianNameArray(VarName);
    } else if (VarTypeString.startswith_lower("char")) {
        return IsHungarianNameChar(VarName);
    } else if (VarTypeString.startswith_lower("struct")) {
        return IsHungarianNameStruct(VarName);
    } else if (VarTypeString.startswith_lower("enum")) {
        return IsHungarianNameEnum(VarName);
    }
    return false;
}

static bool IsHungarianNameFuncPtr(llvm::StringRef VarName) {
    return VarName.endswith_lower("fp") || VarName.startswith_lower("fp");
}

static bool IsHungarianNameChar(llvm::StringRef VarName) {
    return VarName.endswith_lower("chr") || VarName.startswith_lower("chr") ||
           VarName.find_lower("char") != llvm::StringRef::npos;
}

static bool IsHungarianNamePtr(llvm::StringRef VarName) {
    return VarName.endswith_lower("ptr") || VarName.startswith_lower("ptr") ||
           VarName.find_lower("pointer") != llvm::StringRef::npos;
}

static bool IsHungarianNameArray(llvm::StringRef VarName) {
    return VarName.endswith_lower("arr") || VarName.startswith_lower("arr") ||
           VarName.find_lower("array") != llvm::StringRef::npos;
}

static bool IsHungarianNameStruct(llvm::StringRef VarName) {
    return VarName.endswith_lower("str") || VarName.startswith_lower("str") ||
           VarName.find_lower("struct") != llvm::StringRef::npos;
}

static bool IsHungarianNameEnum(llvm::StringRef VarName) {
    return VarName.find_lower("enum") != llvm::StringRef::npos;
}

}  // namespace naming
}  // namespace nett