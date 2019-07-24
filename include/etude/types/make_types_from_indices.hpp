//
//  make_types_from_indices:
//    渡された Indices... を保持する types を生成する
// 
//    etude::make_types_from_indices<Indices> は、型 Indices::type が存在し、
//    その型が etude::indices<Indices....> ならば
//      etude::types<std::integral_constant<std::size_t, Indices>...>
//    を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_MAKE_TYPES_FROM_INDICES_HPP_
#define ETUDE_TYPES_INCLUDED_MAKE_TYPES_FROM_INDICES_HPP_

#include "indices.hpp"
#include "types.hpp"
#include "get_type_or.hpp"

#include <type_traits>

namespace etude {
  
  // 実装
  template<class Indices>
  struct make_types_from_indices_ {};
  
  template<std::size_t... Indices>
  struct make_types_from_indices_< etude::indices<Indices...> >
    : etude::types< std::integral_constant<std::size_t, Indices>... > {};
  
  // 本体
  template<class Indices>
  struct make_types_from_indices
    : make_types_from_indices_<typename get_type_or<Indices>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_MAKE_TYPES_FROM_INDICES_HPP_
