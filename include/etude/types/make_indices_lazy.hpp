//
//  make_indices_lazy:
//    渡された integral_constant に対する indices を生成する
// 
//    etude::make_indices_lazy<T> は、整数定数 T::value が存在する場合には
//    etude::make_indices<T::value>::type を継承し、
//    そうでない場合には etude::void_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_MAKE_INDICES_LAZY_HPP_
#define ETUDE_TYPES_INCLUDED_MAKE_INDICES_LAZY_HPP_

#include <type_traits>
#include "indices.hpp"
#include "null_constant.hpp"

namespace etude {
  
  // 実装
  template<class ValueIsNotDedined, class = void>
  struct make_indices_lazy_
    : etude::void_type {};
  
  template<class IntegralConstant>
  struct make_indices_lazy_< IntegralConstant,
    typename std::enable_if<( IntegralConstant::value >= 0 )>::type
  >
    : etude::make_indices<IntegralConstant::value> {};
  
  // 本体
  // 渡されたクラスが integral constant なら etude::indices<0..N-1> を継承し
  // そうでなければ etude::void_type を継承する
  template<class IntegralConstant>
  struct make_indices_lazy
    : make_indices_lazy_<IntegralConstant>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_MAKE_INDICES_LAZY_HPP_
