//
//  Various data structures which allow for iteration and transformation on iterators.
// Based on Rust's Iterator trait.
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
class Rev;

template<typename B>
class Iterator {
 protected:
  B base_;
 public:
  Iterator(B&& base) : base_(base) {}

  using Item = typename B::Item;
  Maybe<Item> next() { return base_.next(); }

  template<typename F>
  Iterator<Map<B, F>> map(F&& func) {
    return Iterator<Map<B, F>>(Map<B, F>(std::move(*this), std::move(func)));
  }
};

template<typename B>
class DoubleEndedIterator : public Iterator<B> {
 public:
  using Item = typename Iterator<B>::Item;

  DoubleEndedIterator(B&& base) : Iterator<B>(std::move(base)) {}

  Maybe<Item> next_back() { return this->base_.next_back(); }

  Iterator<Rev<B>> rev() {
    return Iterator<Rev<B>>(Rev<B>(std::move(*this)));
  }
};

template<typename B, typename F>
class Map {
  Iterator<B> iter_;
  F func_;

 public:
  Map(Iterator<B>&& it, F&& func) : iter_(it), func_(func) {}

  using BItem = typename Iterator<B>::Item;

  using Item = decltype(std::declval<F>()(std::declval<BItem>()));
  Maybe<Item> next() {
    return iter_.next().map(func_);
  };
};

template<typename B>
class Rev {
  DoubleEndedIterator<B> iter_;

 public:
  using Item = typename B::Item;

  Rev(DoubleEndedIterator<B>&& it) : iter_(it) {}

  using T = RemoveRef<decltype(std::declval<B>().next_back().unwrap())>;
  Maybe<T> next() {
    return iter_.next_back();
  }
};
}
#endif //ACSL_RANGE_HH
