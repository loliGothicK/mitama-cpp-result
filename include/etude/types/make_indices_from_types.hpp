//
//  make_indices_from_types:
//    渡された Indices... を保持する types を生成する
// 
//    etude::make_indices_from_types<Types> は、型 Indices::type が存在し、
//    その型が etude::types<Indices....> ならば
//      etude::indices<Indices::value...>
//    を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_MAKE_INDICES_FROM_TYPES_HPP_
#define ETUDE_TYPES_INCLUDED_MAKE_INDICES_FROM_TYPES_HPP_

#include "indices.hpp"
#include "types.hpp"
#include "make_void.hpp"
#include "get_type_or.hpp"

namespace etude {
  
  // 実装
  template<class Types, class = void>
  struct make_indices_from_types_ {};
  
  template<class... Types>
  struct make_indices_from_types_< etude::types<Types...>,
    typename make_void< etude::indices<Types::value...> >::type
  >
    : etude::indices<Types::value...> {};
  
  // 本体
  template<class Types>
  struct make_indices_from_types
    : make_indices_from_types_<typename get_type_or<Types>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_MAKE_INDICES_FROM_TYPES_HPP_
