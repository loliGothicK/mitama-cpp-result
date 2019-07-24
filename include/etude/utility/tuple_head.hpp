//
//  tuple_head:
//    タプルの先頭の値を取得する
// 
//    etude::tuple_get<0> を呼び出します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TUPLE_HEAD_HPP_
#define ETUDE_UTILITY_INCLUDED_TUPLE_HEAD_HPP_

#include "tuple_get.hpp"

#include "../types/tuple_element.hpp"
#include <utility>

namespace etude {

  // t の先頭要素を get
  template< class Tuple,
    class Result = typename etude::tuple_element<0, Tuple&&>::type
  >
  inline Result tuple_head( Tuple && t ) {
    return tuple_get<0>( std::forward<Tuple>(t) );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TUPLE_HEAD_HPP_
