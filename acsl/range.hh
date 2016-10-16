//
// Created by luminarys on 10/15/16.
//

#ifndef ACSL_RANGE_HH
#define ACSL_RANGE_HH

#include "type_traits.hh"
#include "types.hh"
#include "maybe.hh"

namespace acsl {
    template<typename T>
    class InputRange {
    public:
        virtual void popFront() {};

        virtual Maybe<Ref<T>> front() {};

        virtual bool empty() {};
    };

    template<typename T>
    class ForwardRange : InputRange<T> {
    public:
        ForwardRange<T> save() {};
    };

    template<typename T>
    class BidirectionalRange : ForwardRange<T> {
    public:
        BidirectionalRange<T> save() {};

        virtual Maybe<Ref<T>> back() {};

        virtual void popBack() {};
    };

    template<typename T, typename I>
    constexpr bool IsInputRange = IsBaseOf<InputRange<I>, T>;

    template<typename T, typename I>
    constexpr bool IsForwardRange = IsBaseOf<ForwardRange<I>, T>;

    template<typename T, typename I>
    constexpr bool IsBidirectionalRange = IsBaseOf<BidirectionalRange<I>, T>;
}
#endif //ACSL_RANGE_HH
