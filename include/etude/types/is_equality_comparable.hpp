//
//  is_equality_comparable :
//    == で比較できれば true になるメタ関数
//    
//    etude::is_equality_comparable<T, U> は、
//    T const 型の変数 t および U const 型の変数 u に対して
//    t == u という式が well-formed で、かつその戻り値が bool に明示的に変換できるならば
//    std::true_type を、それ以外の場合には std::false_type を継承します。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_EQUALITY_COMPARABLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_EQUALITY_COMPARABLE_HPP_

#include "is_simply_callable.hpp"
#include "../functional/equal_to.hpp"

namespace etude {

  template<class T, class U = T>
  struct is_equality_comparable :
    etude::is_simply_callable<
      etude::equal_to<T, U>, bool (
        typename std::conditional<std::is_reference<T>::value, T, T const&>::type,
        typename std::conditional<std::is_reference<U>::value, U, U const&>::type
      )
    >::type
  {
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_EQUALITY_COMPARABLE_HPP_
