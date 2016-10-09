//
//  Renamed types.
//

#ifndef ACSL_TYPES_HH
#define ACSL_TYPES_HH

#include <stdint.h>
#include <stddef.h>

// Primitive renames
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = size_t;

// Other renames
template<typename T>
using Ref = std::reference_wrapper<T>;

template<typename T>
using CRef = std::reference_wrapper<const T>;

#endif //ACSL_TYPES_HH
