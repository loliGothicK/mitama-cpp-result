//
//  make_derivable :
//    組み込み型をラップして継承可能にする
//    
//    もし T が直接的に継承可能な型（ class type ）ならば、
//    型 make_derivable<T>::type は T 型になります。
//    T が class type 以外の object type あるいは参照ならば、
//    型 make_derivable<T>::type は etude::wrapper<T> になります。
//    それ以外の場合には、型 make_derivable<T>::type は定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_MAKE_DERIVABLE_HPP_
#define ETUDE_TYPES_INCLUDED_MAKE_DERIVABLE_HPP_

#include <type_traits>
#include "../utility/wrapper.hpp"
#include "is_derivable.hpp"
#include "is_array_of_unknown_bound.hpp"

namespace etude {

  // 実装
  
  // デフォルトでは etude::wrapper<T>
  template<class T, class = void>
  struct make_derivable_
  {
    typedef etude::wrapper<T> type;
  };
  
  // 継承できる場合には T 自身
  template<class T>
  struct make_derivable_< T,
    typename std::enable_if<etude::is_derivable<T>::value>::type
  >
  {
    typedef T type;
  };
  
  // void, 関数, 要素数不明の配列の場合には定義されない
  template<class T>
  struct make_derivable_< T,
    typename std::enable_if<
      std::is_void<T>::value || std::is_function<T>::value ||
      etude::is_array_of_unknown_bound<T>::value
    >::type
  > {};
  
  
  // 本体
  template<class T>
  struct make_derivable
    : make_derivable_<typename std::remove_cv<T>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_MAKE_DERIVABLE_HPP_
