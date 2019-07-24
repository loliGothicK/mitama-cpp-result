//
//  is_unpacked_tuple :
//    unpacked_tuple なら true_type になるメタ関数
//    
//    is_unpacked_tuple<Types...> は、 sizeof...(Types) > 0 かつ
//    typename etude::last_type<Types...>::type が（ cv 修飾された） unpacked_tuple か
//    その参照ならば std::true_type を、そうでなければ std::false_type を継承します。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_UNPACKED_TUPLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_UNPACKED_TUPLE_HPP_

#include <type_traits>
#include "../unpacked_tuple.hpp"

namespace etude {

  template<class... Types>
  struct is_unpacked_tuple;
  
  template<>
  struct is_unpacked_tuple<>
    : std::false_type {};
  
  template<class T>
  struct is_unpacked_tuple<T>
    : std::false_type {};
  
  template<class Tuple, std::size_t... Indices>
  struct is_unpacked_tuple< unpacked_tuple<Tuple, Indices...> >
    : std::true_type {};
  
  template<class T>
  struct is_unpacked_tuple<T const>
    : is_unpacked_tuple<T>::type {};
  template<class T>
  struct is_unpacked_tuple<T volatile>
    : is_unpacked_tuple<T>::type {};
  template<class T>
  struct is_unpacked_tuple<T const volatile>
    : is_unpacked_tuple<T>::type {};
  
  template<class T>
  struct is_unpacked_tuple<T&>
    : is_unpacked_tuple<T>::type {};
  template<class T>
  struct is_unpacked_tuple<T&&>
    : is_unpacked_tuple<T>::type {};
  
  
  template<class T, class... Types>
  struct is_unpacked_tuple<T, Types...>
    : is_unpacked_tuple<Types...>::type {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_UNPACKED_TUPLE_HPP_
