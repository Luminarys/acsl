#ifndef ACSL_TYPE_TRAITS_HH
#define ACSL_TYPE_TRAITS_HH

#include <type_traits>

namespace acsl {
    template<typename, typename>
    constexpr bool IsSame = false;
    template<typename T>
    constexpr bool IsSame<T, T> = false;

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
}

#endif //ACSL_TYPE_TRAITS_HH
