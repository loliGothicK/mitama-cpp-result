//
//  is_in_place_factory:
//    与えられた型が InPlaceFactory かどうかを判定するメタ関数
// 
//    通常はポインタによって dispatch するわけですが、
//    普通に enable_if を使いたい場合もあるので、その時用です。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_MEMORY_INCLUDED_IS_IN_PLACE_FACTORY_HPP_
#define ETUDE_MEMORY_INCLUDED_IS_IN_PLACE_FACTORY_HPP_

#include <boost/utility/in_place_factory.hpp>

#include <type_traits>

namespace etude {

  using boost::in_place_factory_base;
  
  // T が（CV修飾された） InPlaceFactory または InPlaceFactory への参照なら true を返す。
  // そうでなければ false を返す。
  template<class T>
  struct is_in_place_factory
    : std::is_base_of<in_place_factory_base, typename std::remove_reference<T>::type> {};

}

#endif  // #ifndef ETUDE_MEMORY_INCLUDED_IS_IN_PLACE_FACTORY_HPP_
