//
//  pointee_before:
//    ポインタ様のオブジェクトの内部を < で比較する
// 
//    etude::pointee_before( p, q ) は、
//    p および q が共に有効値である場合は bool( *p < *q ) を、
//    p が無効値で q が有効値の場合には false を、
//    それ以外の場合には true を返します。
//  
//    第三引数に関数オブジェクト comp が渡された場合、不等号比較を行う代わりに、
//    渡された関数オブジェクトを用いて比較演算を行います。
//  
//    p および q のどちらかが bool 型に明示的に変換できない場合や、
//    bool( *p < *q ) ないし bool( comp( *p, *q ) ) が ill-formed の場合には、
//    この関数は定義されません。
//  
//    [ note:
//      この順序は、無効値が全ての有効値より「小さく」なる順序で、
//      boost::optional で定義されている比較演算と同じものです。
//    ]
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_POINTEE_BEFORE_HPP_
#define ETUDE_UTILITY_INCLUDED_POINTEE_BEFORE_HPP_

#include <type_traits>
#include <utility>

#include "../types/is_constructible.hpp"
#include "../types/pointee.hpp"
#include "../functional/less.hpp"

namespace etude {

  template< class T, class U, class Comp = etude::less<>,
    class = typename std::enable_if<
      etude::is_constructible< bool,
        decltype(
          std::declval<Comp&>()(
            std::declval<typename etude::pointee<T const&>::type>(),
            std::declval<typename etude::pointee<U const&>::type>()
          )
        )
      >::value
    >::type
  >
  inline bool pointee_before( T const& lhs, U const& rhs, Comp && comp = Comp() ) {
    return rhs ? ( lhs ? bool( comp( *lhs, *rhs ) ) : true ) : false;
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_POINTEE_BEFORE_HPP_
