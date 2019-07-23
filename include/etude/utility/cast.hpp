//
//  cast:
//    引数を指定の型にキャストする
// 
//    渡された引数を、テンプレート引数で与えられた型へ暗黙変換して返す関数です。
// 
//    この関数は、与えられた型が void ならば、単純に引数を無視し void を返し、
//    与えられた型が関数ならば、渡された関数への参照を返し、
//    それ以外の場合には、渡された引数を与えられた型へ暗黙変換して返します。
//    
//    特に、与えられた引数がオブジェクトの場合、この関数の戻り値は prvalue となります。
//    つまり、
//      auto && x = etude::cast<T>(y);
//    と x を定義した場合、 x に束縛されるオブジェクトの寿命は延長されます。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_CAST_HPP_
#define ETUDE_UTILITY_INCLUDED_CAST_HPP_

#include <utility>
#include <type_traits>

namespace etude {

  // for void
  template<class T, class U>
  inline typename std::enable_if<
    std::is_void<T>::value, void >::type
      cast( U&& ) {}
  
  // for functions
  template<class T>
  inline T& cast (
    typename std::enable_if< std::is_function<T>::value, T& >::type x )
  {
    return x;
  }
  
  
  // otherwise
  
  // 一般的な場合。効率が良い。
  template<class T, class U>
  inline typename std::enable_if<
    !std::is_reference<T>::value && std::is_convertible<U, T>::value,
      T >::type cast( U && x )
  {
    return std::forward<U>(x);
  }
  // 上の版で上手く変換できない場合、
  // 例えば cast<void*>(0) のような場合は、こちらが呼ばれる
  // 参照の場合もこちら（一時オブジェクトへの参照を防ぐため）
  template<class T>
  inline T cast( typename std::enable_if<true, T&&>::type x ) {
    return std::forward<T>(x);
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_CAST_HPP_
