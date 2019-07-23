//
//  is_const_lvalue_reference :
//    要素数不明の配列ならば true_type になるメタ関数
//    
//    is_const_lvalue_reference<T> は、 T が cv 修飾されていない型 U を用いて
//    const U& と表せる場合には std::true_type を、
//    そうでない場合には std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_CONST_LVALUE_REFERENCE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_CONST_LVALUE_REFERENCE_HPP_

#include <type_traits>

namespace etude {

  template<class T>
  struct is_const_lvalue_reference
    : std::false_type {};
  
  template<class T>
  struct is_const_lvalue_reference<T const&>
    : std::true_type {};
  
  template<class T>
  struct is_const_lvalue_reference<T const volatile&>
    : std::false_type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_CONST_LVALUE_REFERENCE_HPP_
