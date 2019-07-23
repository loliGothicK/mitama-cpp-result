//
//  last_type:
//    与えられた型リストのうち、最後の引数を返す
// 
//    typename etude::head_of<Ts...>::type は、
//    Ts... が Init... , Last と表せる場合は Last に定義され、
//    Ts... が空の場合には定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_LAST_TYPE_HPP_
#define ETUDE_TYPES_INCLUDED_LAST_TYPE_HPP_

namespace etude {
  
  template<class... Ts>
  struct last_type {};
  
  template<class T>
  struct last_type<T>
  {
    typedef T type;
  };
  
  template<class T, class U, class... Us>
  struct last_type<T, U, Us...>
  {
    typedef typename last_type<U, Us...>::type type;
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_LAST_TYPES_HPP_
