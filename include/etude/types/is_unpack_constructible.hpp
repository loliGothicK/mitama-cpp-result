//
//  is_unpack_constructible :
//    unpack した上で構築可能かを問い合わせる
//    
//    etude::is_unpack_constructible<T, Tuple> は、 Tuple がタプル（への参照）で、かつ
//    Tuple 型の式 t に対し T( UNPACK(t) ) が well-formed なら std::true_type を、
//    そうでなければ std::false_type を継承します。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_UNPACK_CONSTRUCTIBLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_UNPACK_CONSTRUCTIBLE_HPP_

#include <type_traits>

#include "is_constructible.hpp"
#include "get_type_or.hpp"
#include "tuple_types.hpp"

namespace etude {

  // 実装用
  template<class T, class Args>
  struct is_unpack_constructible_
    : std::false_type {};
  
  template<class T, class... Args>
  struct is_unpack_constructible_< T, etude::types<Args...> >
    : etude::is_constructible<T, Args...>::type {};


  // 本体
  template<class T, class Tuple>
  struct is_unpack_constructible :
    is_unpack_constructible_< T,
      typename etude::get_type_or< etude::tuple_types<Tuple> >::type
    >::type
  {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_UNPACK_CONSTRUCTIBLE_HPP_
