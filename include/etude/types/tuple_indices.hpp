//
//  tuple_indices :
//    タプルにアクセスするための indices を作る
//    
//    このメタ関数は、
//      make_indices<etude::tuple_size<T>::value>::type
//    を継承します。
//    
//    ただし、 etude::tuple_size<T>::value が存在しない場合には、
//    etude::void_type を継承します。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TUPLE_INDICES_HPP_
#define ETUDE_TYPES_INCLUDED_TUPLE_INDICES_HPP_

#include "make_indices_lazy.hpp"
#include "tuple_size.hpp"

namespace etude {

  // 単純な実装、 template aliases が使えればなお良いのですが
  template<class T>
  struct tuple_indices
    : make_indices_lazy< etude::tuple_size<T> >::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TUPLE_INDICES_HPP_
