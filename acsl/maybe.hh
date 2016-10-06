#include <utility>

namespace acsl {
template<typename T>
class Maybe {
    union {
        T value_;
        char none_state_;
    };
    bool has_value_ = false;
    public:
        constexpr Maybe(): none_state_('\0') {}
        Maybe(const T& v): value_(v), has_value_(true) {}
        Maybe(T&& v): value_(std::move(v)), has_value_(true) {}
        bool has_value() const {
            return this->has_value_;
        }
        bool is_none() const {
            return !this->has_value_;
        }
        T unwrap_or(const T& v) {
            if (this->has_value_) {
                return std::move(this->value_);
            } else {
                return v;
            }
        }

		template <typename U>
        Maybe<U> map(std::function<U(T)> f) {
            if (this->has_value_) {
                return Maybe(f(std::move(this->value_)));
            } else {
                return Maybe();
            }
        }
};
}
