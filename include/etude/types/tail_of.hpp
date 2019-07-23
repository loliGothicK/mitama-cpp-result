//
//  tail_of:
//    与えられた型リストの最初以外の要素
// 
//    etude::tail_of<Types> は、
//    typename Types::type が etude::types<Head, Tail...> となる場合は
//    etude::types<Tail...> を継承し、
//    それ以外の場合には空のクラスになります。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TAIL_OF_HPP_
#define ETUDE_TYPES_INCLUDED_TAIL_OF_HPP_

#include "types.hpp"
#include "get_type_or.hpp"

namespace etude {
  
  template<class Types>
  class tail_of_ {};
  
  template<class Head, class... Tail>
  struct tail_of_< types<Head, Tail...> >
    : etude::types<Tail...> {};
  
  template<class Types>
  struct tail_of
    : tail_of_<typename get_type_or<Types>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TAIL_OF_HPP_
