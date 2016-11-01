//
// Various data structures which allow for iteration and transformation on iterators.
// Based on Rust's Iterator trait.
//

#ifndef ACSL_ITER_HH
#define ACSL_ITER_HH

#include "type_traits.hh"
#include "types.hh"
#include "maybe.hh"

namespace acsl {

template<typename B>
class RangeLoopIter;

template<typename B, typename F>
class Map;

template<typename B, typename F>
class Filter;

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
    return RangeLoopIter<B>(std::move(*this));
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

  template<typename F>
  Iterator<Filter<B, F>> filter(F&& func) {
    return Iterator<Filter<B, F>>(Filter<B, F>(std::move(*this), std::move(func)));
  }

  template<typename C>
  C collect() {
      return C(std::move(*this));
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
  bool rev_;
 public:
  using Item = T;
  RangeIter(T&& start, T&& end) : start_(start), end_(end), rev_(false) {
    if (start_ > end_) {
      rev_ = true;
      std::swap(start_, end_);
    }
  }

  Maybe<Item> next() {
    if (start_ != end_) {
      if (rev_) {
        return Maybe<Item>(std::move(end_--));
      }
      return Maybe<Item>(std::move(start_++));
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
  Iterator<B> iter_;
  Maybe<Item> next_;

 public:
  RangeLoopIter() {}
  RangeLoopIter(Iterator<B>&& iter)
      : iter_(iter), next_(iter_.next()) {}

  Item operator*() {
    return next_.unwrap();
  }

  bool operator!=(RangeLoopIter&) {
    return next_.has_value();
  }

  void operator++() {
    next_ = iter_.next();
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

template<typename B, typename F>
class Filter {
  Iterator<B> iter_;
  F func_;

 public:
  Filter(Iterator<B>&& it, F&& func) : iter_(it), func_(func) {}

  using Item = typename Iterator<B>::Item;

  Maybe<Item> next() {
    Maybe<Item> cur = nothing;
    do {
      cur = iter_.next();
    } while (!cur.as_ref().map(func_).unwrap_or(true));
    return cur;
  }
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
#endif //ACSL_ITER_HH
