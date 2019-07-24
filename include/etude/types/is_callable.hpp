//
//  is_callable :
//    関数呼び出し可能ならば true_type になるメタ関数
//    
//    is_callable<T, R (Args...)> は、 T 型の t 及び Args... 型の args... に対し
//      etude::invoke<R>( std::forward<T>(t), std::forward<Aegs>(args)... )
//    が well-formed ならば std::true_type を、
//    そうでない場合には std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_CALLABLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_CALLABLE_HPP_

#include <utility>
#include <type_traits>
#include "../functional/invoke.hpp"

namespace etude {

  template<class T, class R, class... Args>
  class is_callable_
  {
    template< class T_ = T,
      class R_ = decltype(
        etude::invoke( std::declval<T_>(), std::declval<Args>()... )
      ),
      class = typename std::enable_if<
        std::is_void<R>::value || std::is_convertible<R_, R>::value
      >::type
    >
    static std::true_type test( int );
    
    static std::false_type test( ... );
    
   public:
    typedef decltype( test(0) ) type;
    
  };
  
  template<class T, class Singature>
  class is_callable {};
  
  template<class T, class R, class... Args>
  struct is_callable<T, R (Args...)>
    : is_callable_<T, R, Args...>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_CALLABLE_HPP_
