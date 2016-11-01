//
// Various data structures which allow for iteration and transformation on iterators.
// Based on Rust's Iterator trait.
//

#ifndef ACSL_RANGE_HH
#define ACSL_RANGE_HH

#include "type_traits.hh"
#include "types.hh"
#include "maybe.hh"

namespace acsl {

template<typename B>
class RangeLoopIter;

template<typename B, typename F>
class Map;

template<typename B>
class Rev;

template<typename B, typename O>
class Chain;

template<typename B>
class Iterator {
 protected:
  B base_;
 public:
  using Item = typename B::Item;

  Iterator(B&& base) : base_(base) {}

  Maybe<Item> next() { return base_.next(); }

  RangeLoopIter<B> begin() {
    return RangeLoopIter<B>(std::move(*this));
  }

  RangeLoopIter<B> end() {
    return RangeLoopIter<B>();
  }

  usize count() {
    usize count = 0;
    while (next().has_value()) {
      count++;
    }
    return count;
  }

  Maybe<Item> last() {
    Maybe<Item> prev = nothing;
    Maybe<Item> cur = next();
    while (cur.has_value()) {
      prev = std::move(cur);
      cur = next();
    }
    return prev;
  }

  Maybe<Item> nth(usize n) {
    Maybe<Item> cur = next();
    while (n-- > 0) {
      cur = next();
    }
    return cur;
  }

  template<typename O>
  Iterator<Chain<B, O>> chain(Iterator<O>&& other) {
    return Iterator<Chain<B, O>>(Chain<B, O>(std::move(*this), std::move(other)));
  }

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

template<typename T>
class RangeIter {
  T start_, end_;
 public:
  using Item = T;
  RangeIter(T&& start, T&& end) : start_(start), end_(end) {}

  Maybe<Item> next() {
    if (start_ != end_) {
      if (start_ < end_) {
        return Maybe<Item>(std::move(start_++));
      } else {
        return Maybe<Item>(std::move(start_--));
      }
    }
    return nothing;
  }
};

template<typename T>
Iterator<RangeIter<T>> range(T&& start, T&& end) {
  return Iterator<RangeIter<T>>(RangeIter<T>(std::move(start), std::move(end)));
}

template<typename T>
Iterator<RangeIter<T>> range(T&& end) {
  return Iterator<RangeIter<T>>(RangeIter<T>(std::move(T()), std::move(end)));
}

template<typename B>
class RangeLoopIter {
  using Item = typename Iterator<B>::Item;
  Maybe<Iterator<B>> iter_;
  Maybe<Item> next_;

 public:
  RangeLoopIter() {}
  RangeLoopIter(Iterator<B>&& iter)
      : iter_(Maybe<Iterator<B>>(std::move(iter))), next_(iter_.as_ref().unwrap().get().next()) {}

  Item operator*() {
    return next_.unwrap();
  }

  bool operator!=(RangeLoopIter&) {
    return next_.has_value();
  }

  void operator++() {
    next_ = iter_.as_ref().unwrap().get().next();
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

  Maybe<Item> next() {
    return iter_.next_back();
  }
};

template<typename B, typename O>
class Chain {
  Iterator<B> first_;
  Iterator<O> second_;
  bool first_valid_;

 public:
  using Item = typename B::Item;

  static_assert(IsSame<Item, typename O::Item>, "Chain must be used with an iterator of the same item type");

  Chain(Iterator<B>&& first, Iterator<O>&& second) : first_(first), second_(second), first_valid_(true) {}

  Maybe<Item> next() {
    if (first_valid_) {
      Maybe<Item> i = first_.next();
      if (i.has_value()) {
        return i;
      }
      first_valid_ = false;
    }
    return second_.next();
  }
};
}
#endif //ACSL_RANGE_HH
