//
//  tail_types:
//    与えられた型リストのうち、最初の引数以外を返す
// 
//    etude::tail_types<Ts...> は、
//    Ts... が Head, Tail... と表せる場合は etude::types<Tail...> を継承し、
//    Ts... が空の場合には空のクラスになります。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TAIL_TYPES_HPP_
#define ETUDE_TYPES_INCLUDED_TAIL_TYPES_HPP_

#include "types.hpp"

namespace etude {
  
  template<class... Ts>
  struct tail_types {};
  
  template<class T, class... Ts>
  struct tail_types<T, Ts...>
    : etude::types<Ts...> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TAIL_TYPES_HPP_
