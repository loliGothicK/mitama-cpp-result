//
//  common_type:
//    std::common_type の改良版
// 
//    etude::common_type<Types...>::type は、
//    etude::has_common_type<Types...>::value が true になる場合には
//    std::common_type<Types...>::type に定義され、それ以外の場合には定義されません。
// 
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_COMMON_TYPE_HPP_
#define ETUDE_TYPES_INCLUDED_COMMON_TYPE_HPP_

#include <type_traits>
#include "has_common_type.hpp"

namespace etude {
  
  // 実装クラス
  template<bool HasCommonType, class... Types>
  struct common_type_
  {
    typedef typename std::common_type<Types...>::type type;
  };
  template<class... Types>
  struct common_type_<false, Types...>
  {
  };
  
  // 本体
  template<class... Types>
  struct common_type
    : common_type_<has_common_type<Types...>::value, Types...> {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_COMPOSITE_POINTER_TYPE_HPP_
