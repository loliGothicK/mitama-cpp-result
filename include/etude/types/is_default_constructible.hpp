//
//  is_default_constructible :
//    std::is_default_constructible の移植版
//    
//    現行の gcc には std::is_default_constructible が無いので、出来るまでのつなぎに。
//    gcc-4.6.0 の std::is_constructible のバグにも対応しています。
//
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_DEFAULT_CONSTRUCTIBLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_DEFAULT_CONSTRUCTIBLE_HPP_

#include <type_traits>

namespace etude {

  // 基本的には std::is_constructible に転送
  template<class T, class = void>
  struct is_default_constructible_
    : std::is_constructible<T> {};
  
  // 与えられた型が void, 関数, 参照 なら false
  // gcc-4.6.0 だと std::is_constructible<void>::value は true だけど、
  // N3242 の is_constructible の定義からすると、 false になるべき。
  template<class T>
  struct is_default_constructible_< T,
    typename std::enable_if<!std::is_object<T>::value>::type
  >
    : std::false_type {};
 
  // scalar type の場合、 gcc-4.6.0 だと SEGV するバグがある。
  // が、 scalar type なら必ず DefaultConstructible なので、自明。
  template<class T>
  struct is_default_constructible_< T,
    typename std::enable_if< std::is_scalar<T>::value >::type
  >
    : std::true_type {};
  
  
  // 本体
  template<class T>
  struct is_default_constructible
    : is_default_constructible_<T>::type {};
  

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_DEFAULT_CONSTRUCTIBLE_HPP_
