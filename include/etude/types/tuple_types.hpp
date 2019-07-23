//
//  tuple_types :
//    タプルに格納された型のリストを作る
//    
//    このメタ関数は、渡された型 Tuple に対し
//    etude::is_tuple<std::decay<Tuple>::type>::value が true になる場合には、
//    etude::types<T0 .. Tn-1> を継承します。
//    ただし T0 .. Tn-1 は etude::tuple_element<I, Tuple>::type に等しい型です。
//    
//    それ以外の場合には、 etude::void_type を継承します。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TUPLE_TYPES_HPP_
#define ETUDE_TYPES_INCLUDED_TUPLE_TYPES_HPP_

#include "types.hpp"
#include "tuple_indices.hpp"
#include "tuple_element.hpp"
#include "null_constant.hpp"

namespace etude {

  // for implementation
  template<class T, class Indices>
  struct tuple_types_ : void_type {};
  
  // Indices を dispatch する
  template<class T, std::size_t... Indices>
  struct tuple_types_< T, etude::indices<Indices...> >
    : types<typename etude::tuple_element<Indices, T>::type...> {};

  // 本体
  template<class T>
  struct tuple_types
    : tuple_types_<T, typename tuple_indices<T>::type>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TUPLE_TYPES_HPP_
