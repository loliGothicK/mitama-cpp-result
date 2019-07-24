//
//  is_decay_copyable :
//    lvalue-to-rvalue conversion が行えるか否か
//    
//    etude::is_decay_copyable<T> は、
//    std::is_constructible<typename std::decay<T>::type, T>::type を継承します。
//    ただし T が void の場合には常に std::false_type を返します。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_DECAY_COPYABLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_DECAY_COPYABLE_HPP_

#include <type_traits>

namespace etude {

  // 基本的には std::is_constructible<typename std::decay<T>::type, T> に転送
  template< class T, class = void >
  struct is_decay_copyable_
    : std::is_constructible<typename std::decay<T>::type, T>::type {};
  
  // ただし void の場合は false
  template< class T >
  struct is_decay_copyable_< T,
    typename std::enable_if<std::is_void<T>::value>::type
  >
    : std::false_type {};
  
  
  // 本体
  template<class T>
  struct is_decay_copyable
    : is_decay_copyable_<T>::type {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_DECAY_COPYABLE_HPP_
