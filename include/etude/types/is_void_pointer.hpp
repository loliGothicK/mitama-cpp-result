//
//  is_void_pointer:
//    void へのポインタなら true になるメタ関数
// 
//    etude::is_void_pointer<T> は、 T が（ cv 修飾された）void へのポインタなら
//    std::true_type を、それ以外なら std::false_type を継承します。
// 
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_VOID_POINTER_HPP_
#define ETUDE_TYPES_INCLUDED_IS_VOID_POINTER_HPP_

#include <type_traits>

namespace etude {
  
  template<class T, class = void>
  struct is_void_pointer_
  {
    typedef std::false_type type;
  };
  template<class T>
  struct is_void_pointer_< T,
    typename std::enable_if< std::is_pointer<T>::value >::type
  >
  {
    typedef typename std::is_void<typename std::remove_pointer<T>::type>::type type;
  };
  
  template<class T>
  struct is_void_pointer
    : is_void_pointer_<T>::type {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_VOID_POINTER_HPP_
