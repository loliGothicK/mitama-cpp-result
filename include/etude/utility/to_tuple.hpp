//
//  to_tuple:
//    渡された Tuple を std::tuple に変換
// 
//    渡されたタプルのようなものを std::tuple に変換します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TO_TUPLE_HPP_
#define ETUDE_UTILITY_INCLUDED_TO_TUPLE_HPP_

#include "../types/tuple_indices.hpp"
#include "../types/tuple_element.hpp"
#include "tuple_get.hpp"

#include <utility>
#include <type_traits>

namespace etude {

  // 実装、兼インデックス指定版
  template< class Tuple, std::size_t... Indices >
  inline std::tuple<
    typename etude::tuple_element< Indices,
      typename std::decay<Tuple>::type
    >::type...
  >
  to_tuple( Tuple && t, etude::indices<Indices...> )
  {
    typedef std::tuple<
      typename etude::tuple_element< Indices,
        typename std::decay<Tuple>::type
      >::type...
    > result_type;
    
    (void)t; // 警告避け
    return result_type( etude::tuple_forward<Indices, Tuple>(t)... );
  }

  // 本体
  template< class Tuple,
    class Result = decltype(
      etude::to_tuple( std::declval<Tuple>(), etude::tuple_indices<Tuple>() )
    )
  >
  inline Result to_tuple( Tuple && t ) {
    return etude::to_tuple( std::forward<Tuple>(t), etude::tuple_indices<Tuple>() );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TO_TUPLE_HPP_
