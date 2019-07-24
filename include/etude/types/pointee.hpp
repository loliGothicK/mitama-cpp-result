//
//  pointee :
//    maybe コンセプトを満たすならば、単項 * 演算子の適用結果を返すメタ関数
//    
//    etude::pointee<T>::type は、 is_maybe<T>::value が true 、即ち T 型の変数 t に対し
//    bool(t) という表現および *std::forward<T>(t) という表現が well-formed ならば
//    decltype( *std::forward<T>(t) ) に定義され、それ以外の場合には定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_POINTEE_HPP_
#define ETUDE_TYPES_INCLUDED_POINTEE_HPP_

#include <utility>
#include <type_traits>
#include "is_maybe.hpp"

namespace etude {

  template<class T, class = void>
  struct pointee_ {};
  
  template<class T>
  struct pointee_< T,
    typename std::enable_if<
      etude::is_maybe<T>::value
    >::type
  >
  {
    typedef decltype( *std::declval<T>() ) type;
  };
  
  template<class T>
  struct pointee
    : pointee_<T> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_POINTEE_HPP_
