/*
 * Result type implementation, inspired by Rust.
 */

#ifndef ACSL_RESULT_HH
#define ACSL_RESULT_HH

#include <functional>
#include "type_traits.hh"
#include "maybe.hh"

namespace acsl {
    template<typename T, typename E>
    class Result {
        union {
            T value_;
            E error_;
        };
        bool has_value_;
    public:
        static_assert(!IsSame<T, E>);

        Result(T&& v) : value_(std::move(v)), has_value_(true) {}

        Result(E&& v) : error_(std::move(v)), has_value_(false) {}

        bool is_ok() const {
            return this->has_value_;
        }

        bool is_err() const {
            return !this->has_value_;
        }

        Maybe<T> ok() {
            if (this->has_value_) {
                this->has_value_ = false;
                return Maybe<T>(std::move(this->value_));
            } else {
                return Maybe<T>();
            }
        }

        Maybe<E> err() {
            if (this->has_value_) {
                return Maybe<E>();
            } else {
                return Maybe<E>(std::move(this->error_));
            }
        }

        Result<const T&, const E&> as_ref() const {
            if (this->has_value_) {
                return Result<const T&, const E&>(this->value_);
            } else {
                return Result<const T&, const E&>(this->error_);
            }
        }

        T unwrap_or(T v) {
            if (this->has_value_) {
                return std::move(this->value_);
            } else {
                return v;
            }
        }

        T unwrap_or_else(std::function<T(E)> f) {
            if (this->has_value_) {
                return this->value_;
            } else {
                return f(this->error_);
            }
        }

        template<typename U>
        Result<U, E> and_then(std::function<Result<U, E>(T)> f) {
            if (this->has_value_) {
                return f(this->value_);
            } else {
                return Result<U, E>(this->error_);
            }
        }

        template<typename U>
        Result<U, E> map(std::function<U(T)> f) {
            if (this->has_value_) {
                return Result<U, E>(f(std::move(this->value_)));
            } else {
                return Result<U, E>(std::move(this->error_));
            }
        }
    };
}
#endif
