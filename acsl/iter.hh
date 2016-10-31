//
//  Various data structures which encompass range types.
// Based on D-lang's concept of ranges.
//

#ifndef ACSL_RANGE_HH
#define ACSL_RANGE_HH

#include "type_traits.hh"
#include "types.hh"
#include "maybe.hh"

namespace acsl {

    template<typename B, typename F>
    class Map;

    template<typename B>
    class Iterator {
        B base_;
    public:
        Iterator(B&& base) : base_(base) {}

        using T = RemoveRef<decltype(std::declval<B>().next().unwrap())>;

        Maybe <T> next() { return base_.next(); }

        template<typename F>
        Iterator<Map<B, F>> map(F&& func) {
            return Iterator<Map<B, F>>(Map<B, F>(std::move(*this), std::move(func)));
        }
    };

    template<typename B, typename F>
    class Map {
        using R = decltype(std::declval<F>()(std::declval<Iterator<B>>().next().unwrap()));

        Iterator<B> iter_;
        F func_;

    public:
        Map(Iterator<B>&& it, F&& func) : iter_(it), func_(func) {}

        Maybe <MapResult<R, F>> next() {
            return iter_.next().map(func_);
        };
    };
}
#endif //ACSL_RANGE_HH
