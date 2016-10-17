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
    template<typename B, typename T>
    struct RangeIterator;

    template<typename B, typename T>
    class InputRange {
    public:
        virtual void popFront() {};

        virtual Maybe<Ref<T>> front() {};

        virtual Maybe<T> moveFront() {};

        virtual bool empty() {};

        virtual RangeIterator<B, T> begin() {
            return RangeIterator<B, T>(static_cast<B *>(this));
        }

        virtual RangeIterator<B, T> end() {
            return RangeIterator<B, T>();
        }
    };

    template<typename B, typename T>
    class ForwardRange : public InputRange<B, T> {
    public:
        ForwardRange<B, T> save() {};
    };

    template<typename B, typename T>
    class BidirectionalRange : public ForwardRange<B, T> {
    public:
        BidirectionalRange<B, T> save() {};

        virtual Maybe<Ref<T>> back() {};

        virtual Maybe<T> moveBack() {};

        virtual void popBack() {};
    };

    template<typename T>
    struct PointerRange : public BidirectionalRange<PointerRange<T>, T> {
        T *start_;
        T *end_;

    public:
        PointerRange(T *start, T *end) : start_(start), end_(end) {}

        void popFront() {
            start_++;
        }

        Maybe<T> moveFront() {
            if (empty()) {
                return nothing;
            }
            T&& v = std::move(*start_++);
            return Maybe<T>(std::move(v));
        }

        void popBack() {
            end_--;
        }

        Maybe<T> moveBack() {
            if (empty()) {
                return nothing;
            }
            T&& v = std::move(*end_--);
            return Maybe<T>(std::move(v));
        }

        Maybe<Ref<T>> front() const {
            if (empty()) {
                return nothing;
            }
            return Maybe<Ref<T>>(std::ref(*start_));
        }

        Maybe<Ref<T>> back() const {
            if (empty()) {
                return nothing;
            }
            return Maybe<Ref<T>>(std::ref(*end_));
        }

        bool empty() const {
            return start_ == end_;
        }

        PointerRange<T> save() const {
            return VecIter(start_, end_);
        }
    };

    template<typename B, typename T>
    struct RangeIterator {
        B *range_;
        bool start_;

        RangeIterator() : start_(false) {}

        RangeIterator(B *range) : range_(range), start_(true) {}

        RangeIterator& operator++() {
            range_->popFront();
        }

        bool operator!=(RangeIterator) {
            return !range_->empty();
        }

        Ref<T> operator*() {
            return range_->front().unwrap();
        }
    };


    template<typename T, typename I>
    constexpr bool IsInputRange = IsBaseOf<InputRange<T, I>, T>;

    template<typename T, typename I>
    constexpr bool IsForwardRange = IsBaseOf<ForwardRange<T, I>, T>;

    template<typename T, typename I>
    constexpr bool IsBidirectionalRange = IsBaseOf<BidirectionalRange<T, I>, T>;
}
#endif //ACSL_RANGE_HH
