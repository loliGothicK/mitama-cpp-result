//
//  identity_before:
//    与えられたオブジェクトのアドレスの順序を比較する関数
// 
//    与えられたオブジェクトのアドレスを std::less で比較します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_IDENTITY_BEFORE_HPP_
#define ETUDE_UTILITY_INCLUDED_IDENTITY_BEFORE_HPP_

#include <boost/utility/addressof.hpp>
#include "less_pointer.hpp"

namespace etude {

  // 基本的にはこちらが呼ばれる
  template< class T, class U,
    class = decltype(
      etude::less_pointer( std::declval<T*>(), std::declval<U*>() )
    )
  >
  inline bool identity_before( T& lhs, U& rhs ) {
    return etude::less_pointer( boost::addressof(lhs), boost::addressof(rhs) );
  }
  
  // 型を指定した場合はこっち
  // const& ではなく const volatile& なのは、一時オブジェクトを bind しないための工夫
  template<class T>
  inline bool identity_before( T const volatile& lhs, T const volatile& rhs ) {
    return etude::less_pointer( boost::addressof(lhs), boost::addressof(rhs) );
  }


} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_IDENTITY_BEFORE_HPP_
