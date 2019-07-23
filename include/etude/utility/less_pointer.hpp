//
//  less_pointer:
//    与えられたポインタを比較する関数
// 
//    与えられたポインタを std::less で比較します。
//    これにより、その結果は total order となります。
//    ポインタの型が違う場合には composite pointer type に変換してから比較します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_LESS_POINTER_HPP_
#define ETUDE_UTILITY_INCLUDED_LESS_POINTER_HPP_

#include <functional> // for std::less
#include "../types/common_type.hpp"

namespace etude {

  // std::less で比較する
  template<class T>
  inline bool less_pointer( T* lhs, T* rhs ) {
    return std::less<T*>()( lhs, rhs );
  }

  // 型が違う場合は common_type で比較する
  // 無関係のポインタの場合は SFINAE される
  template< class T, class U,
    class P = typename etude::common_type<T*, U*>::type
  >
  inline bool less_pointer( T* lhs, U* rhs ) {
    return std::less<P>()( lhs, rhs );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_LESS_POINTER_HPP_
