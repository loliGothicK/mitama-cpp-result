//
//  head_type:
//    与えられた型リストの最初の型
// 
//    typename etude::head_type<Ts...>::type は、
//    Ts... が Head, Tail... と表せる場合は Head に定義され、
//    Ts... が空の場合には定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_HEAD_TYPE_HPP_
#define ETUDE_TYPES_INCLUDED_HEAD_TYPE_HPP_

namespace etude {
  
  template<class... Ts>
  struct head_type {};
  
  template<class T, class... Ts>
  struct head_type<T, Ts...>
  {
    typedef T type;
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_HEAD_TYPE_HPP_
