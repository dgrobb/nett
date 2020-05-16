/* Copyright (C) 2020  Matthys Grobbelaar
 *  Full license notice can be found in Nett.cpp
 */
#include "PointerStyles.hpp"

namespace nett {
namespace checks {
namespace whitespace {

// We start off without any particular pointer style in use.
enum PointerStyle PtrStyle = PTR_UNSET;

void SetPointerStyle(enum PointerStyle Style) {
    if (PtrStyle == PointerStyle::PTR_UNSET) {
        PtrStyle = Style;
    }
}

}  // namespace whitespace
}  // namespace checks
}  // namespace nett
