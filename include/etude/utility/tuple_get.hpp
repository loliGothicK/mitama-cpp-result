//
//  tuple_get:
//    タプルからの値の get
// 
//    ・現行の gcc が && 版の std::get に対応していないので、それに対応
//    ・直接 std::get って呼ぶと std::get が dependent name にならないので
//      std::array 等を後から include した場合に std::get が見つからない問題にも対処
//  
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TUPLE_GET_HPP_
#define ETUDE_UTILITY_INCLUDED_TUPLE_GET_HPP_

#include "../types/tuple_element.hpp"
#include <utility>
#include <type_traits>

namespace etude {

  // t の I 番目の要素を get
  template< std::size_t I, class Tuple,
    class Result = typename etude::tuple_element<I, Tuple&&>::type
  >
  inline Result tuple_get( Tuple && t ) {
    return etude::tuple_element<I, Tuple&&>::get( std::forward<Tuple>(t) );
  }
  
  // 要素を move しつつ get する
  template< std::size_t I, class Tuple,
    class Result = decltype (
      tuple_get<I>( std::move( std::declval<Tuple>() ) )
    )
  >
  inline Result tuple_move( Tuple && t ) {
    return tuple_get<I>( std::move(t) );
  }
  
  // 要素を Tuple の型に合わせて forward してから get する
  template< std::size_t I, class Tuple, class Tuple_,
    class Result = decltype(
      tuple_get<I>( std::forward<Tuple>( std::declval<Tuple_>() ) )
    )
  >
  inline Result tuple_forward( Tuple_ && t ) {
    return tuple_get<I>( std::forward<Tuple>(t) );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TUPLE_GET_HPP_
