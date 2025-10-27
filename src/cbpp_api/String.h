#ifndef CBPP_STRING_API_H
#define CBPP_STRING_API_H

#include "cbpp/const_string.h"
#include "cbpp/immutable_string.h"
#include "cbpp/mutable_string.h"

// Constant compile-time string
#define CbCS(str) cbpp::CConstString(str)

// Immutable heap-allocated string
#define CbIS(str) cbpp::CImmutableString(str);

// Mutable heap-allocated string
#define CbMS(str) cbpp::CString(str);

namespace cbpp {

}

#endif
