/*
 *  Maybe type implementation, inspired by Rust and OctaSTD.
 */

#ifndef ACSL_MAYBE_HH
#define ACSL_MAYBE_HH

#include "type_traits.hh"
#include "types.hh"

namespace acsl {

    struct Nothing {
        explicit constexpr Nothing() {}
    };

    constexpr Nothing nothing = Nothing();

    template<typename T>
    class Maybe {
        union {
            T value_;
            char none_state_;
        };
        bool has_value_ = false;
    public:
        static_assert(!IsSame<T, Nothing>);

        constexpr Maybe() : none_state_('\0') {}

        constexpr Maybe(Nothing) : none_state_('\0') {}

        Maybe(Maybe const&) = delete;

        Maybe(Maybe&& v) : has_value_(v.has_value_) {
            if (has_value_) {
                value_ = std::move(v.value_);
            }
        }

        Maybe& operator=(Maybe const& other)  = delete;

        Maybe& operator=(Maybe&& other) {
            this->has_value_ = other.has_value_;
            if (other.has_value_) {
                this->value_ = std::move(other.value_);
            }
            return *this;
        }

        Maybe(T&& v) : value_(std::move(v)), has_value_(true) {}

        friend bool operator==(Maybe<T> const& a, Maybe<T> const& b) {
            if (a.has_value() && b.has_value()) {
                return a.value_ == b.value_;
            }
            return !a.has_value() && !b.has_value();
        }

        friend bool operator==(Maybe<T> const& a, Nothing const& b) {
            return a.is_none();
        }

        friend bool operator==(Maybe<T> const& a, T const& b) {
            if (a.has_value()) {
                return a.value_ == b;
            }
            return false;
        }

        ~Maybe() {}

        constexpr bool has_value() const {
            return this->has_value_;
        }

        constexpr bool is_none() const {
            return !this->has_value_;
        }

        T&& unwrap() {
            if (this->has_value_) {
                this->has_value_ = false;
                return std::move(this->value_);
            } else {
                exit(0);
            }
        }

        T&& unwrap_or(T&& v) {
            if (this->has_value_) {
                this->has_value_ = false;
                return std::move(this->value_);
            } else {
                return std::move(v);
            }
        }

        Maybe<CRef<T>> as_cref() const {
            if (this->has_value_) {
                return Maybe<CRef<T>>(std::cref(this->value_));
            } else {
                return nothing;
            }
        }

        Maybe<Ref<T>> as_ref() {
            if (this->has_value_) {
                return Maybe<Ref<T>>(std::ref(this->value_));
            } else {
                return nothing;
            }
        }

        template<typename F>
        using RV = decltype(std::declval<F>()(std::declval<T>()));

        template<typename F>
        Maybe<RV<F>> map(F&& f) {
            if (this->has_value_) {
                this->has_value_ = false;
                return Maybe<RV < F>>
                (f(std::move(this->value_)));
            } else {
                return nothing;
            }
        }

        template<typename F>
        RV<F> map_or(RV<F>&& v, F&& f) {
            if (this->has_value_) {
                this->has_value_ = false;
                return f(std::move(this->value_));
            } else {
                return v;
            }
        }

        template<typename F>
        Maybe<RV<F>> and_then(F&& f) {
            if (this->has_value_) {
                this->has_value_ = false;
                return f(std::move(this->value_));
            } else {
                return nothing;
            }
        }
    };
}
#endif
