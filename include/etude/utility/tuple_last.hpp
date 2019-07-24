//
//  tuple_last:
//    タプルの最後の値を取得する
// 
//    etude::tuple_get<etude::tuple_size<Tuple>::value - 1> を呼び出します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TUPLE_LAST_HPP_
#define ETUDE_UTILITY_INCLUDED_TUPLE_LAST_HPP_

#include "tuple_get.hpp"
#include "../types/tuple_size.hpp"

#include "../types/tuple_element.hpp"
#include <utility>

namespace etude {

  // t の末尾要素を get
  template< class Tuple,
    std::size_t N = etude::tuple_size<Tuple>::value - 1,
    class Result = typename etude::tuple_element<N, Tuple&&>::type
  >
  inline Result tuple_last( Tuple && t ) {
    return tuple_get<N>( std::forward<Tuple>(t) );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TUPLE_LAST_HPP_
