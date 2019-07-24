//
//  compare_not_equal_to:
//    与えられた引数に対し != を適用する
// 
//    与えられた引数に対し != 演算子を用いた比較ができる場合は、
//    その結果を bool に明示的に変換して返します。
//  
//    それ以外の場合、もし == 演算子による比較が出来る場合には、
//    その結果に対し ! 演算子を適用して返します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_COMPARE_NOT_EQUAL_TO_HPP_
#define ETUDE_UTILITY_INCLUDED_COMPARE_NOT_EQUAL_TO_HPP_

#include <utility>
#include <type_traits>
#include "compare_equal_to.hpp"
#include "../types/is_constructible.hpp"

namespace etude {


  // 実装の実装
  
  // T と U が、この順に比較可能な場合
  template< class T, class U,
    class R = decltype(
      std::declval<T>() != std::declval<U>()
    ),
    class = typename std::enable_if< etude::is_constructible<bool, R>::value >::type
  >
  inline R compare_not_equal_to_impl2_( T && lhs, U && rhs, int ) {
    return std::forward<T>(lhs) != std::forward<U>(rhs);
  }
  // T と U が、順序を逆転すると比較可能になる場合は順序反転
  template< class T, class U,
    class R = decltype(
      std::declval<U>() != std::declval<T>()
    ),
    class = typename std::enable_if< etude::is_constructible<bool, R>::value >::type
  >
  inline R compare_not_equal_to_impl2_( T && lhs, U && rhs, ... ) {
    return std::forward<U>(rhs) != std::forward<T>(lhs);
  }
  

  // 実装用
  
  // ポインタ以外で、 != による比較が可能な場合
  template< class T, class U,
    class = typename std::enable_if<
      !( std::is_pointer<typename std::decay<T>::type>::value &&
         std::is_pointer<typename std::decay<U>::type>::value )
    >::type,
    class R = decltype(
      etude::compare_not_equal_to_impl2_( std::declval<T>(), std::declval<U>(), 0 )
    ),
    class = typename std::enable_if< etude::is_constructible<bool, R>::value >::type
  >
  inline R compare_not_equal_to_impl_( T && lhs, U && rhs, int ) {
    return etude::compare_not_equal_to_impl2_(
      std::forward<T>(lhs), std::forward<U>(rhs), 0
    );
  }
  
  // compare_equal_to による比較が可能な場合
  template< class T, class U,
    class R = decltype(
      !etude::compare_equal_to_( std::declval<T>(), std::declval<U>() )
    ),
    class = typename std::enable_if< etude::is_constructible<bool, R>::value >::type
  >
  inline R compare_not_equal_to_impl_( T && lhs, U && rhs, ... ) {
    return !etude::compare_equal_to_( std::forward<T>(lhs), std::forward<U>(rhs) );
  }
  
  
  // 戻り値を bool にキャストしない版
  template< class T, class U,
    class R = decltype(
      etude::compare_not_equal_to_impl_( std::declval<T>(), std::declval<U>(), 0 )
    ),
    class = typename std::enable_if< etude::is_constructible<bool, R>::value >::type
  >
  inline R compare_not_equal_to_( T && lhs, U && rhs ) {
    return etude::compare_not_equal_to_impl_(
      std::forward<T>(lhs), std::forward<U>(rhs), 0
    );
  }
  
  
  // 戻り値を bool にキャストする版
  
  // 同じ型の場合
  template< class T,
    class = decltype(
      etude::compare_not_equal_to_( std::declval<T const&>(), std::declval<T const&>() )
    )
  >
  inline bool compare_not_equal_to( T const& lhs, T const& rhs ) {
    return bool( etude::compare_not_equal_to_( lhs, rhs ) );
  }
  // 異なる型の場合
  template< class T, class U,
    class = decltype(
      etude::compare_not_equal_to_( std::declval<T>(), std::declval<U>() )
    )
  >
  inline bool compare_not_equal_to( T && lhs, U && rhs ) {
    return bool(
      etude::compare_not_equal_to_( std::forward<T>(lhs), std::forward<U>(rhs) )
    );
  }


} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_COMPARE_NOT_EQUAL_TO_HPP_
