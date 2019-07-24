//
//  by_val:
//    引数を値渡しする旨をコード上で明記
// 
//    与えられた引数が rvalue ならば、与えられた引数への rvalue reference を、
//    lvalue ならば、与えられた引数への const lvalue reference を返す関数です。
//    
//    これにより、最大限の効率を保ったまま、
//    変数に束縛されたオブジェクトに対する関数内部からの書き換えを防ぎます。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_BY_VAL_HPP_
#define ETUDE_UTILITY_INCLUDED_BY_VAL_HPP_

#include <type_traits>

namespace etude {

  template<class T>
  inline typename std::conditional<
    std::is_lvalue_reference<T>::value,
    typename std::remove_reference<T>::type const&,
    T&&
  >::type by_val( T && x ) {
    return static_cast<T&&>(x);
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_BY_VAL_HPP_
