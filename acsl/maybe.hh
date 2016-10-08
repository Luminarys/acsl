/*
 *  Maybe type implementation, inspired by Rust and OctaSTD.
 */

#ifndef ACSL_MAYBE_HH
#define ACSL_MAYBE_HH

#include <functional>
#include "type_traits.hh"

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

        constexpr Maybe(Maybe&& v) : has_value_(v.has_value_) {
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

        constexpr Maybe(T&& v) : value_(std::move(v)), has_value_(true) {}

        ~Maybe() {}

        constexpr bool has_value() const {
            return this->has_value_;
        }

        constexpr bool is_none() const {
            return !this->has_value_;
        }

        T unwrap_or(T&& v) {
            if (this->has_value_) {
                this->has_value_ = false;
                return std::move(this->value_);
            } else {
                return std::move(v);
            }
        }

        template<typename U>
        Maybe<U> map(std::function<U(T)> f) {
            if (this->has_value_) {
                this->has_value_ = false;
                return Maybe<U>(f(std::move(this->value_)));
            } else {
                return Maybe<U>();
            }
        }

        template<typename U>
        U map_or(U v, std::function<U(T)> f) {
            if (this->has_value_) {
                this->has_value_ = false;
                return f(std::move(this->value_));
            } else {
                return v;
            }
        }

        template<typename U>
        Maybe<U> and_then(std::function<Maybe<U>(T)> f) {
            if (this->has_value_) {
                this->has_value_ = false;
                return f(std::move(this->value_));
            } else {
                return Maybe<U>();
            }
        }
    };
}
#endif
