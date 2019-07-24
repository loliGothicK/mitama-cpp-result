//
//  size_of :
//    sizeof(T) を返すメタ関数
//    
//    size_of<T> は、 std::remove_reference<T>::type を U としたとき、
//    U が要素数不定の配列を除いた object type ならば、
//    std::integral_constant<std::size_t, sizeof(U)> を継承します。
//    そうでなければ size_of<T> は std::integral_constant<std::size_t, 0> を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_SIZE_OF_HPP_
#define ETUDE_TYPES_INCLUDED_SIZE_OF_HPP_

#include <type_traits>
#include "size_constant.hpp"
#include "is_array_of_unknown_bound.hpp"

namespace etude {

  // for implementation
  template<class T, class = void>
  struct size_of_ 
    : etude::size_constant<0>::type
  {
    static bool const is_defined = false;
  };
  
  template<class T>
  struct size_of_< T,
    typename std::enable_if<
      std::is_object<T>::value && !is_array_of_unknown_bound<T>::value
    >::type
  > : etude::size_constant<sizeof(T)>::type
  {
    static bool const is_defined = true;
  };
  
  // 本体
  template<class T>
  struct size_of
    : size_of_<typename std::remove_reference<T>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_SIZE_OF_HPP_
