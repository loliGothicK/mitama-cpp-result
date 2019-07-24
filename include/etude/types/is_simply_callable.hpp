//
//  is_simply_callable :
//    関数呼び出し可能ならば true_type になるメタ関数
//    
//    is_simply_callable<T, R (Args...)> は、 T 型の t 及び Args... 型の args... に対し
//    式 std::forward<T>(t)( std::forward<Args>(args)... ) が well-formed で
//    その結果が R 型へと暗黙変換できる場合には std::true_type を、
//    そうでない場合には std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_SIMPLY_CALLABLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_SIMPLY_CALLABLE_HPP_

#include <utility>
#include <type_traits>

namespace etude {

  template<class T, class R, class... Args>
  class is_simply_callable_
  {
    template< class F = T,
      typename R_ = decltype(
        std::declval<F>()( std::declval<Args>()... )
      ),
      class = typename std::enable_if<
        std::is_convertible<R_, R>::value ||
        std::is_void<R>::value
      >::type
    >
    static std::true_type test_( int );
    
    static std::false_type test_( ... );
    
   public:
    typedef decltype( test_(0) ) type;
    
  };
  
  template<class T, class Singature>
  class is_simply_callable {};
  
  template<class T, class R, class... Args>
  struct is_simply_callable<T, R (Args...)>
    : is_simply_callable_<T, R, Args...>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_SIMPLY_CALLABLE_HPP_
