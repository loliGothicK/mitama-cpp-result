//
//  align_of :
//    alignof(T) を返すメタ関数
//    
//    align_of<T> は、 std::remove_reference<T>::type を U としたとき、
//    U が object type ならば std::integral_constant<std::size_t, alignof(U)> を、
//    そうでなければ std::integral_constant<std::size_t, 0> を継承します。
//    
//    std::alignment_of との違いは、 T が void や関数であっても動作する点です。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_ALIGN_OF_HPP_
#define ETUDE_TYPES_INCLUDED_ALIGN_OF_HPP_

#include <type_traits>
#include "size_constant.hpp"
#include "is_array_of_unknown_bound.hpp"

namespace etude {

  // for implementation
  template<class T, class = void>
  struct align_of_
    : etude::size_constant<0>::type
  {
    static bool const is_defined = false;
  };
  
  // 本当は alignof(T[]) は T と同じになる筈だけど gcc 4.5.0 ではコンパイルエラー。
  template<class T>
  struct align_of_< T,
    typename std::enable_if<
      std::is_object<T>::value && !is_array_of_unknown_bound<T>::value
    >::type
  > : etude::size_constant<alignof(T)>::type
  {
    static bool const is_defined = true;
  };
  // なので分ける
  template<class T>
  struct align_of_<T[]>
    : align_of_<T> {};
  
  // 本体
  template<class T>
  struct align_of
    : align_of_<typename std::remove_reference<T>::type> {};
  

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_ALIGN_OF_HPP_
