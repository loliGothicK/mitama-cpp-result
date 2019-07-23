//
//  nth_of:
//    与えられた型リストの N+1 番目の要素
// 
//    typename etude::nth_of<N, Types>::type は、
//    typename Types::type が etude::types<Ts...> となり、かつ
//    N < sizeof...(Ts) の場合には Types... の N 番目の型（ N は 0 ベース）に定義され、
//    それ以外の場合には定義されません。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_NTH_OF_HPP_
#define ETUDE_TYPES_INCLUDED_NTH_OF_HPP_

#include "nth_type.hpp"

#include "types.hpp"
#include "get_type_or.hpp"

namespace etude {
  
  template<std::size_t N, class Ts>
  struct nth_of_ {};
  
  template<std::size_t N, class... Ts>
  struct nth_of_< N, types<Ts...> >
    : nth_type<N, Ts...> {};
  
  template<std::size_t N, class Types>
  struct nth_of
    : nth_of_<N, typename get_type_or<Types>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_NTH_OF_HPP_
