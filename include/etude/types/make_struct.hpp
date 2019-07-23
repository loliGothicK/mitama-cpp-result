//
//  make_struct :
//    T 型を保持した構造体を返すメタ関数
//    
//    もし T が参照または要素数不明の配列を除く object type ならば
//    make_struct<T>::type は T 型を保持した匿名の構造体になります。
//    そうでない場合、 make_struct<T>::type は定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_MAKE_STRUCT_HPP_
#define ETUDE_TYPES_INCLUDED_MAKE_STRUCT_HPP_

#include <type_traits>
#include "is_array_of_unknown_bound.hpp"

namespace etude {

  // 実装用
  template<class T, class = void>
  struct make_struct_ {};

  template<class T>
  struct make_struct_< T,
    typename std::enable_if<
      std::is_reference<T>::value ||
      ( std::is_object<T>::value && !is_array_of_unknown_bound<T>::value )
    >::type
  >
  {
    typedef struct { T x; } type;
  };
  
  // 本体
  template<class T>
  struct make_struct
    : make_struct_<T> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_MAKE_STRUCT_HPP_
