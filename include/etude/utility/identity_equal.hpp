//
//  identity_equal:
//    与えられたオブジェクトの同一性を比較する関数
// 
//    与えられたオブジェクトのアドレスを == で比較します。
//    これにより、与えられたオブジェクトが同じオブジェクトならば true になります。
//  
//    [ Example:
//        int i = 0, j = 0;
//        int const &ri = i;
//        etude::identity_equal( i, i );   // true
//        etude::identity_equal( i, j );   // false
//        etude::identity_equal( i, ri );  // true
//        etude::identity_equal( i, 0 );   // ill-formed
//        etude::identity_equal( ri, 0 );  // ill-formed
//        
//    ]
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_IDENTITY_EQUAL_HPP_
#define ETUDE_UTILITY_INCLUDED_IDENTITY_EQUAL_HPP_

#include <boost/utility/addressof.hpp>
#include "../types/is_equality_comparable.hpp"

namespace etude {

  // 基本的にはこちらが呼ばれる
  template< class T, class U,
    class = typename std::enable_if<
      etude::is_equality_comparable<T*, U*>::value
    >::type
  >
  inline bool identity_equal( T& lhs, U& rhs ) {
    return boost::addressof(lhs) == boost::addressof(rhs);
  }
  
  // 型を指定した場合はこっち
  // const& ではなく const volatile& なのは、一時オブジェクトを bind しないための工夫
  template<class T>
  inline bool identity_equal( T const volatile& lhs, T const volatile& rhs ) {
    return boost::addressof(lhs) == boost::addressof(rhs);
  }


} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_IDENTITY_EQUAL_HPP_
