//
// Templated safe array class.
//

#ifndef ACSL_ARRAY_HH
#define ACSL_ARRAY_HH

#include <initializer_list>
#include "types.hh"
#include "maybe.hh"

namespace acsl {
    template<typename T, usize N>
    class Array {
        static_assert(N > 0);
        T buffer_[N];
    public:
        Array(std::initializer_list<T> l) {
            int i = 0;
            for (T e: l) {
                buffer_[i++] = e;
            }
        }

        Maybe<Ref<T>> operator[](usize i) {
            if (i >= N) {
                return nothing;
            } else {
                return Maybe<Ref<T>>(std::ref(buffer_[i]));
            }
        }

        Maybe<CRef<T>> operator[](usize i) const {
            if (i >= N) {
                return nothing;
            } else {
                return Maybe<CRef<T>>(std::cref(buffer_[i]));
            }
        }

        template<usize I>
        T& at() {
            static_assert(I < N);
            return buffer_[I];
        }

        template<usize I>
        T const& at() const {
            static_assert(I < N);
            return buffer_[I];
        }

        T& front() {
            return this->at<0>();
        }

        T const& front() const {
            return this->at<0>();
        }

        T& back() {
            return this->at<N - 1>();
        }

        T const& back() const {
            return this->at<N - 1>();
        }
    };
}

#endif //ACSL_ARRAY_HH
