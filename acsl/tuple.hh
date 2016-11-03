//
// Tuple type.
//

#ifndef ACSL_TUPLE_HH
#define ACSL_TUPLE_HH

#include "types.hh"

namespace acsl {
namespace detail {

template<usize... Indices>
struct IndexSeq {
  using Type = IndexSeq<Indices...>;
};

template<usize I, typename Sequence>
struct CatIndexSeq;

template<usize I, usize... Indices>
struct CatIndexSeq<I, IndexSeq<Indices...>> : IndexSeq<Indices..., I> {
};

template<usize N>
struct MakeIndexSeq : CatIndexSeq<N - 1, typename MakeIndexSeq<N - 1>::Type>::Type {
};

template<>
struct MakeIndexSeq<1> : IndexSeq<0> {};

template<usize I, typename T>
struct TupleElement {
  T value_;
};

template<typename Sequences, typename... Types>
struct TupleImpl;

template<usize... Indices, typename... Types>
struct TupleImpl<IndexSeq<Indices...>, Types...> : TupleElement<Indices, Types> ... {

};
}

template<typename... Types>
class Tuple : detail::TupleImpl<typename detail::MakeIndexSeq<sizeof...(Types)>::Type, Types...> {
 public:
  constexpr Tuple(Types&& ... args) {
  }
};
}

#endif //ACSL_TUPLE_HH
