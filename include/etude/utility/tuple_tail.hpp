//
//  tuple_tail:
//    タプルの先頭の値を除いたタプルを取得する
// 
//    std::tuple<Head, Tails...> から std::tuple<Tails...> を得ます。
//    実際には std::tuple 以外も対応できます。
//    空 tuple の場合には SFINAE によって候補から外されます。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TUPLE_TAIL_HPP_
#define ETUDE_UTILITY_INCLUDED_TUPLE_TAIL_HPP_

#include "../types/tuple_indices.hpp"
#include "../types/tuple_element.hpp"
#include "tuple_get.hpp"

#include <utility>
#include <type_traits>

namespace etude {

  // 実装
  template< class Tuple, std::size_t Head, std::size_t... Tails >
  inline std::tuple<
    typename etude::tuple_element< Tails,
      typename std::decay<Tuple>::type
    >::type...
  >
  tuple_tail_( Tuple && t, etude::indices<Head, Tails...> )
  {
    typedef std::tuple<
      typename etude::tuple_element< Tails,
        typename std::decay<Tuple>::type
      >::type...
    > result_type;
    
    return result_type( etude::tuple_forward<Tails, Tuple>(t)... );
  }

  // t の先頭要素以外を詰めた tuple を得る
  template< class Tuple,
    class Result = decltype(
      etude::tuple_tail_( std::declval<Tuple>(), etude::tuple_indices<Tuple>() )
    )
  >
  inline Result tuple_tail( Tuple && t ) {
    return etude::tuple_tail_( std::forward<Tuple>(t), etude::tuple_indices<Tuple>() );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TUPLE_TAIL_HPP_
