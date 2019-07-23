//
//  head_of:
//    与えられた型リストの最初の要素
// 
//    typename etude::head_of<Types>::type は、
//    typename Types::type が etude::types<Head, Tail...> となる場合は Head に定義され、
//    それ以外の場合には定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_HEAD_OF_HPP_
#define ETUDE_TYPES_INCLUDED_HEAD_OF_HPP_

#include "types.hpp"
#include "get_type_or.hpp"

namespace etude {
  
  template<class Types>
  class head_of_ {};
  
  template<class Head, class... Tail>
  struct head_of_< types<Head, Tail...> >
  {
    typedef Head type;
  };
  
  template<class Types>
  struct head_of
    : head_of_<typename get_type_or<Types>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_HEAD_OF_HPP_
