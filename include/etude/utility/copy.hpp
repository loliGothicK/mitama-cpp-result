//
//  copy:
//    与えられた引数をコピーする関数
// 
//    与えられた引数をコピーした prvalue を返します。
//    
//    この関数の戻り値の型は、引数を t としたとき、
//    auto x = t; によって作られる x と同じ型になります。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_COPY_HPP_
#define ETUDE_UTILITY_INCLUDED_COPY_HPP_

#include <type_traits>
#include "../types/is_decay_copyable.hpp"

namespace etude {

  template< class T >
  inline typename std::enable_if<
    etude::is_decay_copyable<T>::value,
    typename std::decay<T>::type
  >::type copy( T && x ) {
    return static_cast<T&&>(x);
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_COPY_HPP_
