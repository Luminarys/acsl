#ifndef ACSL_TYPE_TRAITS_HH
#define ACSL_TYPE_TRAITS_HH

namespace acsl {
    template<typename, typename>
    constexpr bool IsSame = false;
    template<typename T>
    constexpr bool IsSame<T, T> = false;
}

#endif //ACSL_TYPE_TRAITS_HH
