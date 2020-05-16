/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#ifndef LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_WHITESPACE_POINTERSTYLES_HPP
#define LLVM_CLANG_TOOLS_EXTRA_NETT_CHECKS_WHITESPACE_POINTERSTYLES_HPP

namespace nett {
namespace checks {
namespace whitespace {

enum PointerStyle {
    PTR_LEFT,   // left aligned pointers (e.g. int *x)
    PTR_RIGHT,  // right aligned pointers (e.g. int* x)
    PTR_UNSET   // no pointer style found yet
};

// Sets the pointer style to use while checking.
// If the pointer style has already been set, it is unchanged.
void SetPointerStyle(enum PointerStyle Style);

// We need to keep track of the pointer style in use for all source files.
// The global variable allows us to perform pointer style checks
// without having to inject the variable into any classes.
extern enum PointerStyle PtrStyle;

}  // namespace whitespace
}  // namespace checks
}  // namespace nett

#endif