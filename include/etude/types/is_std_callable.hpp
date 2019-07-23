//
//  is_std_callable :
//    関数呼び出し可能ならば true_type になるメタ関数
//    
//    is_std_callable<T, R (Args...)> は、 T 型の t 及び Args... 型の args... に対し
//      etude::std_invoke<R>( std::forward<T>(t), std::forward<Aegs>(args)... )
//    が well-formed ならば std::true_type を、
//    そうでない場合には std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_STD_CALLABLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_STD_CALLABLE_HPP_

#include <utility>
#include "../functional/wrap_if_mem_fn.hpp"
#include "is_simply_callable.hpp"

namespace etude {

  template<class T, class Singature>
  class is_std_callable {};
  
  template<class T, class R, class... Args>
  struct is_std_callable<T, R (Args...)> :
    etude::is_simply_callable<
      decltype( etude::wrap_if_mem_fn( std::declval<T>() ) ),
      R (Args...)
    >::type
  {
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_STD_CALLABLE_HPP_
