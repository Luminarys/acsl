//
// Dynamic resizing array implementation.
//

#ifndef ACSL_VECTOR_HH
#define ACSL_VECTOR_HH

#include "iter.hh"
#include "maybe.hh"
#include "type_traits.hh"
#include "types.hh"
#include <cstring>
#include <initializer_list>
#include <memory>

namespace acsl {
template<typename T>
class Iter;

template<typename T>
class CIter;

template<typename T, typename A>
class IntoIter;

template<typename T, typename A = std::allocator<T>>
class Vector {
  static_assert(IsPod<T> || IsNoThrowCopyConstructible<T>, "Vector type must be POD or NoThrowCopyConstructible");
  static_assert(IsNoThrowCopyAssignable<T>, "Vector type must be NoThrowCopyAssignable");
  static_assert(IsNoThrowMoveConstructible<T>, "Vector type must be NoThrowMoveConstructible");
  static_assert(IsNoThrowMoveAssignable<T>, "Vector type must be NoThrowMoveAssignable");

  usize size_, capacity_;
  A allocator_;
  T *buffer_;

 public:
  Vector(A const &a = A()) :
      size_(0), capacity_(0), buffer_(nullptr), allocator_(a) {
  }

  Vector(usize n, T const &val = T(), A const &a = A()) :
      Vector(a) {
    if (n == 0) {
      return;
    }
    buffer_ = allocator_.allocate(n);
    size_ = capacity_ = n;

    if (IsPod<T>) {
      for (int i = 0; i < n; i++) {
        memcpy(&buffer_[i], &val, sizeof(T));
      }
    } else {
      for (int i = 0; i < n; i++) {
        allocator_.construct(&buffer_[i], val);
      }
    }
  }

  Vector(Vector const &v, A const &a = A()) :
      size_(v.size_), capacity_(v.size_), buffer_(nullptr),
      allocator_(a) {
    buffer_ = allocator_.allocate(v.size_);
    copy_contents(v);
  }

  Vector(Vector &&v, A const &a = A()) :
      size_(v.size_), capacity_(v.size_), buffer_(nullptr), allocator_(a) {
    buffer_ = allocator_.allocate(v.size_);
    for (int i = 0; i < v.size_; i++) {
      allocator_.construct(&buffer_[i], std::move(v.buffer_[i]));
    }
  }

  Vector(std::initializer_list<T> l, A const &a = A()) :
      size_(l.size()), capacity_(l.size()), buffer_(nullptr), allocator_(a) {
    buffer_ = allocator_.allocate(l.size());
    int i = 0;
    for (T e : l) {
      allocator_.construct(&buffer_[i++], std::move(e));
    }
  }

  ~Vector() {
    for (int i = 0; i < size_; i++) {
      allocator_.destroy(&buffer_[i]);
    }
    allocator_.deallocate(buffer_, capacity_);
  }

  Vector &operator=(Vector const &v) {
    clear();
    allocator_.deallocate(buffer_, capacity_);
    allocator_.allocate(buffer_, v.size_);
    size_ = v.size_;
    capacity_ = v.size_;
    copy_contents(v);
    return *this;
  }

  Vector &operator=(Vector &&v) {
    std::swap(buffer_, v.buffer_);
    std::swap(size_, v.size_);
    std::swap(capacity_, v.capacity_);
    std::swap(allocator_, v.allocator_);
    return *this;
  }

  usize capacity() const {
    return capacity_;
  }

  usize size() const {
    return size_;
  }

  void insert(usize idx, T &&v) {
    shift_right(idx, 1);
    buffer_[idx] = std::move(v);
  }

  void insert(usize idx, T const &v) {
    shift_right(idx, 1);
    buffer_[idx] = v;
  }

  void push(T const &v) {
    if (size_ == capacity_) {
      resize(capacity_ * 1.5);
    }
    allocator_.construct(&buffer_[size_++], v);
  }

  void push(T &&v) {
    if (size_ == capacity_) {
      resize(capacity_ * 1.5);
    }
    allocator_.construct(&buffer_[size_++], std::move(v));
  }

  Maybe<T> pop() {
    if (size_ == 0) {
      return nothing;
    }
    T o = T(std::move(buffer_[size_ - 1]));
    allocator_.destroy(&buffer_[size_ - 1]);
    size_--;
    return Maybe<T>(std::move(o));
  }

  Maybe<T> pop_front() {
    if (size_ == 0) {
      return nothing;
    }
    T o = T(std::move(buffer_[0]));
    size_--;
    return Maybe<T>(std::move(o));
  }

  Maybe<Ref<T>> first() {
    return elem_at(0);
  }

  Maybe<CRef<T>> first() const {
    return elem_at(0);
  }

  Maybe<Ref<T>> last() {
    return elem_at(size_ - 1);
  }

  Maybe<CRef<T>> last() const {
    return elem_at(size_ - 1);
  }

  Maybe<Ref<T>> operator[](usize i) {
    return elem_at(i);
  }

  Maybe<CRef<T>> operator[](usize i) const {
    return elem_at(i);
  }

  void clear() {
    for (int i = 0; i < size_; i++) {
      allocator_.destroy(&buffer_[i]);
    }
    size_ = 0;
  }

  Iterator<Iter<T>> iter() {
    return Iterator<Iter<T>>(Iter<T>(buffer_, buffer_ + size_));
  }

  Iterator<CIter<T>> citer() const {
    return Iterator<CIter<T>>(CIter<T>(buffer_, buffer_ + size_));
  }

  Iterator<IntoIter<T, A>> into_iter() {
    return Iterator<IntoIter<T, A>>(IntoIter<T, A>(std::move(this)));
  }

 private:
  Maybe<Ref<T>> elem_at(usize i) {
    if (i >= size_) {
      return nothing;
    }
    return Maybe<Ref<T>>(std::ref(buffer_[i]));
  }

  Maybe<CRef<T>> elem_at(usize i) const {
    if (i >= size_) {
      return nothing;
    }
    return Maybe<CRef<T>>(std::cref(buffer_[i]));
  }

  void resize(usize n, T const &v = T()) {
    if (n == 0) {
      clear();
      return;
    }
    usize psize = size_;
    size_ = n;
    reserve(n);
    if (IsPod<T>) {
      for (usize i = psize; i < size_; i++) {
        buffer_[i] = T(v);
      }
    } else {
      for (usize i = psize; i < size_; i++) {
        allocator_.construct(&buffer_[i], v);
      }
    }
  }

  void reserve(usize n) {
    if (n > capacity_) {
      T *new_buffer = allocator_.allocate(n);
      for (int i = 0; i < size_; i++) {
        allocator_.construct(&new_buffer[i], std::move(buffer_[i]));
        allocator_.destroy(&buffer_[i]);
      }
      allocator_.deallocate(buffer_, capacity_);
      buffer_ = new_buffer;
      capacity_ = n;
    }
  }

  void shift_right(usize idx, usize n) {
    if (size_ + n > capacity_) {
      resize(size_ + n);
    }
    size_ += n;
    for (usize i = size_ - 1; i > idx + n - 1; --i) {
      buffer_[i] = std::move(buffer_[i - n]);
    }
  }

  // Shifts elements left n positions
  void shift_left(usize n) {
    size_ -= n;
    for (usize i = 0; i < n; i++) {
      buffer_[i] = std::move(buffer_[i + n]);
    }
    for (usize i = size_ - n; i < size_; i++) {
      allocator_.destroy(&buffer_[i]);
    }
  }

  void copy_contents(Vector const &v) {
    if (IsPod<T>) {
      memcpy(buffer_, v.buffer_, sizeof(T) * size_);
    } else {
      for (int i = 0; i < size_; i++) {
        allocator_.construct(&buffer_[i], v.buffer_[i]);
      }
    }
  }
};

template<typename T>
class Iter {
  T *start_;
  T *end_;

 public:
  Iter(T *start, T *end)
      :
      start_(start), end_(end) {
  }

  Maybe<Ref<T>> next() {
    if (start_ != end_) {
      return Maybe<Ref<T>>(std::ref(*start_++));
    }
    return nothing;
  }
};

template<typename T>
class CIter {
  T *start_;
  T *end_;

 public:
  CIter(T *start, T *end)
      :
      start_(start), end_(end) {
  }

  Maybe<CRef<T>> next() {
    if (start_ != end_) {
      return Maybe<CRef<T>>(std::cref(*start_++));
    }
    return nothing;
  }
};

template<typename T, typename A>
class IntoIter {
  Vector<T, A> vec_;

 public:
  IntoIter(Vector<T, A> vec)
      :
      vec_(vec) {
  }

  Maybe<T> next() {
    return vec_.pop_front();
  }
};
}

#endif //ACSL_VECTOR_HH
