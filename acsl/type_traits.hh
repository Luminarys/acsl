#ifndef ACSL_TYPE_TRAITS_HH
#define ACSL_TYPE_TRAITS_HH

#include <type_traits>

namespace acsl {
template<typename, typename>
constexpr bool IsSame = false;
template<typename T>
constexpr bool IsSame<T, T> = true;

template<typename T>
constexpr bool IsPod = std::is_pod<T>::value;

template<typename T>
constexpr bool IsNoThrowCopyConstructible = std::is_nothrow_copy_constructible<T>::value;

template<typename T>
constexpr bool IsNoThrowCopyAssignable = std::is_nothrow_copy_assignable<T>::value;

template<typename T>
constexpr bool IsNoThrowMoveConstructible = std::is_nothrow_move_constructible<T>::value;

template<typename T>
constexpr bool IsNoThrowMoveAssignable = std::is_nothrow_move_assignable<T>::value;

template<typename T, typename U>
constexpr bool IsConvertible = std::is_convertible<T, U>::value;

template<typename T, typename U>
constexpr bool IsBaseOf = std::is_base_of<T, U>::value;

template<typename T, typename F>
using MapResult = decltype(std::declval<F>()(std::declval<T>()));

template<typename T>
using RemoveRef = typename std::remove_reference<T>::type;
}

#endif //ACSL_TYPE_TRAITS_HH
