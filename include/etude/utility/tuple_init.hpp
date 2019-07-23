//
//  tuple_init:
//    タプルの最後の値を除いたタプルを取得する
// 
//    std::tuple<Init..., Last> から std::tuple<Init...> を得ます。
//    実際には std::tuple 以外も対応できます。
//    空 tuple の場合には SFINAE によって候補から外されます。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TUPLE_INIT_HPP_
#define ETUDE_UTILITY_INCLUDED_TUPLE_INIT_HPP_

#include "../types/indices.hpp"
#include "../types/tuple_size.hpp"
#include "to_tuple.hpp"

#include <utility>
#include <type_traits>

namespace etude {

  // t の末尾要素以外を詰めた tuple を得る
  template< class Tuple,
    class Indices = etude::make_indices<etude::tuple_size<Tuple>::value - 1>,
    class Result = decltype( etude::to_tuple( std::declval<Tuple>(), Indices() ) )
  >
  inline Result tuple_init( Tuple && t ) {
    return etude::to_tuple( std::forward<Tuple>(t), Indices() );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TUPLE_INIT_HPP_
