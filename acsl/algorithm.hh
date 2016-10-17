//
// Various algorithms which can operate on ranges.
//

#ifndef ACSL_ALGORITHM_HH
#define ACSL_ALGORITHM_HH

#include "range.hh"

namespace acsl {

    template<typename R, typename F, typename T>
    struct RangeMap : public InputRange<RangeMap<R, F, T>, T> {
        F func_;
        R range_;
        T temp_;

        RangeMap(R range, F func) : range_(range), func_(func) {}

        void popFront() {
            range_.popFront();
        };

        Maybe <Ref<T>> front() {
            if (empty()) {
                return nothing;
            }
            temp_ = func_(range_.front().unwrap());
            return Maybe<Ref<T>>(std::ref(temp_));
        };

        Maybe <T> moveFront() {
            if (empty()) {
                return nothing;
            }
            return Maybe<T>(std::move(func_(range_.moveFront().unwrap())));
        }

        bool empty() {
            return range_.empty();
        };
    };

    template<typename R>
    using RangeVal = decltype(std::declval<R>().front().unwrap());

    template<typename R, typename F>
    using UTR = decltype(std::declval<F>()(std::declval<RangeVal<R>>()));

    template<typename R, typename F>
    RangeMap<R, F, UTR<R, F>> map(R range, F func) {
        return RangeMap<R, F, UTR<R, F>>(range, func);
    }
}


#endif //ACSL_ALGORITHM_HH
