//
//  by_ref:
//    引数を参照渡しする旨をコード上で明記
// 
//    与えられた引数が非 const の lvalue ならば、与えられた引数を返します。
//    それ以外の場合には、プログラムは ill-formed になります。
//    
//    これにより、関数に対し、引数を参照渡しする旨を明記できます。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_BY_REF_HPP_
#define ETUDE_UTILITY_INCLUDED_BY_REF_HPP_

#include <type_traits>

namespace etude {

  // const でない lvalue reference ならば、与えられた引数を返す
  template<class T>
  inline typename std::enable_if<
    !std::is_const<T>::value,
    T&
  >::type by_ref( T& x ) {
    return x;
  }
  // そうでない場合には ill-formed
  template<class T>
  inline void by_ref( T const& x ) = delete;

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_BY_REF_HPP_
