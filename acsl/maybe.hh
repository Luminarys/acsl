/*
 *  Maybe type implementation, inspired by Rust.
 */

#ifndef ACSL_MAYBE_HH
#define ACSL_MAYBE_HH

#include <functional>

namespace acsl {
    template<typename T>
    class Maybe {
        union {
            T value_;
            char none_state_;
        };
        bool has_value_ = false;
    public:
        constexpr Maybe() : none_state_('\0') {}

        Maybe(T&& v) : value_(std::move(v)), has_value_(true) {}

        bool has_value() const {
            return this->has_value_;
        }

        bool is_none() const {
            return !this->has_value_;
        }

        T unwrap_or(T&& v) {
            if (this->has_value_) {
                this->has_value_ = false;
                return std::move(this->value_);
            } else {
                return v;
            }
        }

        Maybe<const T&> as_ref() const {
            if (this->has_value_) {
                return Maybe<const T&>(this->value_);
            } else {
                return Maybe<const T&>();
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
