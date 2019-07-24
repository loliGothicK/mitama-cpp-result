//
//  is_maybe :
//    maybe コンセプトを満たすならば true_type になるメタ関数
//    
//    etude::is_maybe<T> は、 T 型が maybe concept を満たす、即ち T 型の変数 t に対し
//    bool(t) という表現および *std::forward<T>(t) という表現が well-formed ならば
//    std::true_type を、そうでなければ std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_MAYBE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_MAYBE_HPP_

#include <utility>
#include <type_traits>
#include "is_constructible.hpp"

namespace etude {

  template<class T, class = void>
  struct is_maybe_
    : std::false_type {};
  
  template<class T>
  class is_maybe_< T,
    typename std::enable_if<
      etude::is_constructible<bool, T&>::value
    >::type
  >
  {
    template< class T_ = T,
      class = decltype( *std::declval<T_>() )
    >
    static std::true_type test_( int );
    
    static std::false_type test_( ... );
    
   public:
    typedef decltype( test_(0) ) type;
    
  };
  
  template<class T>
  struct is_maybe
    : is_maybe_<T>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_MAYBE_HPP_
