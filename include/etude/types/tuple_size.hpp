//
//  tuple_size :
//    std::tuple_size の参照対応版
//    
//    perfect-forward やってると、 tuple を参照で受けることも多いので、
//    その場合にいちいち remove_reference するのが面倒で作ったもの。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TUPLE_SIZE_HPP_
#define ETUDE_TYPES_INCLUDED_TUPLE_SIZE_HPP_

#include <type_traits>
#include <tuple>

#include "is_tuple.hpp"
#include "size_constant.hpp"
#include "null_constant.hpp"

namespace etude {

  // 実装補助
  // tuple じゃなければ etude::null_constant<std::size_t>
  template<class T, class = void>
  struct tuple_size_
    : etude::null_constant<std::size_t> {};
  // tuple ならば std::tuple_size<T>
  // 何故か gcc 4.5.0 では std::tuple_size が integral_constant でなかったので
  // 明示的に integral_constant に
  template<class T>
  struct tuple_size_< T,
    typename std::enable_if<etude::is_tuple<T>::value>::type
  >
    : etude::size_constant<std::tuple_size<T>::value> {};

  // 本体
  // 上記のメタ関数を、参照と CV 修飾子を消し飛ばした上で適用
  template<class T>
  struct tuple_size
    : etude::tuple_size_<T>::type {};
  
  template<class T>
  struct tuple_size<T const>
    : etude::tuple_size<T>::type {};
  template<class T>
  struct tuple_size<T volatile>
    : etude::tuple_size<T>::type {};
  template<class T>
  struct tuple_size<T const volatile>
    : etude::tuple_size<T>::type {};
  
  template<class T>
  struct tuple_size<T&>
    : etude::tuple_size<T>::type {};
  template<class T>
  struct tuple_size<T&&>
    : etude::tuple_size<T>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TUPLE_SIZE_HPP_
