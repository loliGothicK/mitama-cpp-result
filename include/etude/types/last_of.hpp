//
//  last_of:
//    与えられた型リストの最後の要素
// 
//    typename etude::last_of<Types>::type は、
//    typename Types::type が etude::types<Init... , Last> となる場合は Last に定義され、
//    それ以外の場合には定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_LAST_OF_HPP_
#define ETUDE_TYPES_INCLUDED_LAST_OF_HPP_

#include "last_type.hpp"

#include "types.hpp"
#include "get_type_or.hpp"

namespace etude {
  
  template<class Ts>
  struct last_of_ {};
  
  template<class T, class... Ts>
  struct last_of_< types<T, Ts...> >
  {
    typedef typename last_type<T, Ts...>::type type;
  };
  
  template<class Types>
  struct last_of
    : last_of_<typename get_type_or<Types>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_LAST_OF_HPP_
