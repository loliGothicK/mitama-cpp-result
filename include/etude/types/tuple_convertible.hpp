//
//  tuple_convertible :
//    etude::tuple_convert を呼べるか否か
//    
//    etude::tuple_convertible<From, To> は、 From 型の式 from に対し、
//    etude::tuple_convert<To>( from ) が well-formed ならば std::true_type を、
//    そうでなければ std::false_type を継承します。
//
//    特に etude::tuple_convertible<std::tuple<FromTypes...>, std::tuple<ToTypes...>> は
//    sizeof...(FromTypes) == sizeof...(ToTypes) かつ
//    std::is_convertible<FromTypes, ToTypes>... が全て true ならば std::true_type を、
//    そうでなければ std::false_type を継承します。
//
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TUPLE_CONVERTIBLE_HPP_
#define ETUDE_TYPES_INCLUDED_TUPLE_CONVERTIBLE_HPP_

#include <type_traits>

#include "types.hpp"
#include "all_of_c.hpp"
#include "tuple_size.hpp"
#include "tuple_types.hpp"

namespace etude {

  // 実装用
  
  // デフォルト
  template<class FromTypes, class ToTypes, class = void>
  struct tuple_convertible_impl_
    : std::false_type {};
  
  // 要素数が一致する場合
  template<class... FromTypes, class... ToTypes>
  struct tuple_convertible_impl_<
    etude::types<FromTypes...>, etude::types<ToTypes...>,
    typename std::enable_if<
      sizeof...(FromTypes) == sizeof...(ToTypes)
    >::type
  >
    : etude::all_of_c<std::is_convertible<FromTypes, ToTypes>::value...> {};
  
  
  // タプルか否か、参照か否かで、もう一段はさませる
  template<class From, class To, class = void>
  struct tuple_convertible_
    : std::false_type {};
  
  template<class From, class To>
  struct tuple_convertible_< From, To,
    typename std::enable_if<
      !std::is_reference<To>::value &&
      ( etude::tuple_size<From>::value == etude::tuple_size<To>::value )
    >::type
  >
    : tuple_convertible_impl_<
        typename etude::tuple_types<From>::type,
        typename etude::tuple_types<To>::type
      >
  {
  };
  
  
  // 本体
  template<class From, class To>
  struct tuple_convertible
    : tuple_convertible_<From, To>::type {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TUPLE_CONVERTIBLE_HPP_
