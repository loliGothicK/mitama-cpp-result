//
//  tuple_convert:
//    渡された Tuple を別の Tuple へ変換
// 
//    渡されたタプルのようなものを別のタプルへ変換します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TUPLE_CONVERT_HPP_
#define ETUDE_UTILITY_INCLUDED_TUPLE_CONVERT_HPP_

#include "../types/tuple_convertible.hpp"
#include "../types/tuple_indices.hpp"
#include "tuple_get.hpp"

#include <utility>
#include <type_traits>

namespace etude {

  template< class To, class From, std::size_t... Indices >
  inline To tuple_convert_( From && t, etude::indices<Indices...> ) {
    return To( etude::tuple_forward<Indices, From>(t)... );
  }
  
  template< class To, class From,
    class = typename std::enable_if<etude::tuple_convertible<From, To>::value>::type
  >
  inline To tuple_convert( From && t ) {
    return etude::tuple_convert_<To>(
      std::forward<From>(t), etude::tuple_indices<From>()
    );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TUPLE_CONVERT_HPP_
