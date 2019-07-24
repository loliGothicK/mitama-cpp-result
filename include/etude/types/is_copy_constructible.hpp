//
//  is_copy_constructible :
//    std::is_copy_constructible の移植版
//    
//    現行の gcc には std::is_copy_constructible が無いので、出来るまでのつなぎに。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_COPY_CONSTRUCTIBLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_COPY_CONSTRUCTIBLE_HPP_

#include "is_object_or_reference.hpp"
#include "is_constructible.hpp"
#include <type_traits>

namespace etude {

  // 基本的には etude::is_constructible<T, T const&> に転送
  template<class T, class = void>
  struct is_copy_constructible_
    : etude::is_constructible<T, T const&>::type {};
  
  // T が void, 関数の場合には false
  template<class T>
  struct is_copy_constructible_< T,
    typename std::enable_if< !etude::is_object_or_reference<T>::value >::type
  >
    : std::false_type {};
  
  
  // 本体
  template<class T>
  struct is_copy_constructible
    : is_copy_constructible_<T> {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_DEFAULT_CONSTRUCTIBLE_HPP_
