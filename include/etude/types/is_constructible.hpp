//
//  is_constructible :
//    std::is_constructible のバグ修正版
//    
//    gcc-4.6.0 の std::is_constructible のバグに対応。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_CONSTRUCTIBLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_CONSTRUCTIBLE_HPP_

#include "is_default_constructible.hpp"
#include "is_object_or_reference.hpp"
#include "any_of_c.hpp"
#include <type_traits>

namespace etude {

  // 実装の実装
  
  // 基本的には std::is_constructible に転送
  template<class Void, class T, class... Args>
  struct is_constructible_impl_
    : std::is_constructible<T, Args...>::type {};
  
  // 引数の中に一つでも void があれば、 false に
  template<class T, class... Args>
  struct is_constructible_impl_<
    typename std::enable_if<
      etude::any_of_c<std::is_void<Args>::value...>::value
    >::type, T, Args...
  >
    : std::false_type {};


  // 実装用
  
  // 基本的には is_constructible_impl_（つまり std::is_constructible ） へと転送
  template<class Void, class T, class... Args>
  struct is_constructible_
    : is_constructible_impl_<void, T, Args...>::type {};
  
  // T が void, 関数の場合は false
  template<class T, class... Args>
  struct is_constructible_<
    typename std::enable_if< !etude::is_object_or_reference<T>::value >::type,
    T, Args...
  >
    : std::false_type {};
  
  // gcc の実装バグへの対応
  
  // 二引数で T, U が共に scalar type の場合は std::is_convertible へと転送
  template<class T, class U>
  struct is_constructible_<
    typename std::enable_if<
      std::is_scalar<T>::value &&
      std::is_scalar<typename std::decay<U>::type>::value
    >::type,
    T, U
  >
    : std::is_convertible<U, T>::type {};
  
  // 二引数で T が参照の場合にも std::is_convertible へと転送
  template<class T, class U>
  struct is_constructible_<
    typename std::enable_if< std::is_reference<T>::value >::type,
    T, U
  >
    : std::is_convertible<U, T>::type {};
  
  // 二引数以上で T が scalar type や参照の場合には std::false_type に決め打ちする
  template<class T, class... Args>
  struct is_constructible_<
    typename std::enable_if<
      ( sizeof...(Args) > 1 ) &&
      ( std::is_scalar<T>::value || std::is_reference<T>::value )
    >::type,
    T, Args...
  >
    : std::false_type {};


  // 本体は、基本的には is_constructible_ に転送
  template<class T, class... Args>
  struct is_constructible
    : is_constructible_<void, T, Args...>::type {};
  
  // ただし default constructible の場合はそっちに。
  template<class T>
  struct is_constructible<T>
    : etude::is_default_constructible<T>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_CONSTRUCTIBLE_HPP_
