//
//  init_of:
//    与えられた型リストの最後以外の要素
// 
//    etude::init_of<Types> は、
//    typename Types::type が etude::types<Init... , Last> となる場合は
//    etude::types<Init...> を継承し、
//    それ以外の場合には空のクラスになります。
// 
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_INIT_OF_HPP_
#define ETUDE_TYPES_INCLUDED_INIT_OF_HPP_

#include "init_types.hpp"

#include "types.hpp"
#include "get_type_or.hpp"

namespace etude {
  
  template<class Ts>
  struct init_of_ {};
  
  template<class T, class... Ts>
  struct init_of_< types<T, Ts...> >
    : init_types<T, Ts...>::type {};
  
  template<class Types>
  struct init_of
    : init_of_<typename get_type_or<Types>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_INIT_OF_HPP_
