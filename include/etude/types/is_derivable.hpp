//
//  is_derivable :
//    派生できるか否か
//    
//    etude::is_derivable<T> は、 T が継承できる型ならば std::true_type を、
//    継承できない型ならば std::false_type を継承します。
//
//    etude::is_derivable<T>::value は
//      std::is_class<T>::value || std::is_union<T>::value
//    と等しくなります。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_DERIVABLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_DERIVABLE_HPP_

#include <type_traits>
#include "bool_constant.hpp"

namespace etude {

  template<class T>
  struct is_derivable
    : bool_constant< std::is_class<T>::value || std::is_union<T>::value >::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_DERIVABLE_HPP_
