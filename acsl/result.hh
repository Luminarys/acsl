/*
 * Result type implementation, inspired by Rust.
 */

#ifndef ACSL_RESULT_HH
#define ACSL_RESULT_HH

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

  constexpr Result(Result&& o) : has_value_(o.has_value_) {
    if (has_value_) {
      value_ = std::move(o.value_);
    } else {
      error_ = std::move(o.error_);
    }
  }

  Result(Result const&) = delete;

  constexpr Result(T&& v) : value_(std::move(v)), has_value_(true) {}

  constexpr Result(E&& v) : error_(std::move(v)), has_value_(false) {}

  ~Result() {}

  Result& operator=(Result const& other)  = delete;

  Result& operator=(Result&& other) {
    this->has_value_ = other.has_value_;
    if (other.has_value_) {
      this->value_ = std::move(other.value_);
    } else {
      this->error_ = std::move(other.error_);
    }
    return *this;
  }

  constexpr bool is_ok() const {
    return this->has_value_;
  }

  constexpr bool is_err() const {
    return !this->has_value_;
  }

  Maybe <T> ok() {
    if (this->has_value_) {
      return Maybe<T>(std::move(this->value_));
    } else {
      return Maybe<T>();
    }
  }

  Maybe <E> err() {
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

  template<typename F>
  T unwrap_or_else(F&& f) {
    if (this->has_value_) {
      return this->value_;
    } else {
      return f(this->error_);
    }
  }

  template<typename U, typename F>
  Result<U, E> and_then(F&& f) {
    if (this->has_value_) {
      return f(this->value_);
    } else {
      return Result<U, E>(this->error_);
    }
  }

  template<typename U, typename F>
  Result<U, E> map(F&& f) {
    if (this->has_value_) {
      return Result<U, E>(f(std::move(this->value_)));
    } else {
      return Result<U, E>(std::move(this->error_));
    }
  }
};
}
#endif
