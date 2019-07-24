//
//  is_assignable_or_convertible :
//    etude::is_assignable または std::is_convertible
//    
//    etude::is_assignable_or_convertible<T, U> は、
//      etude::is_assignable<T, U>::value || std::is_convertible<U, T>::value
//    が true の場合は std::true_type を、
//    それ以外の場合には std::false_type を継承します。
//
//    主に etude::optional で使います。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_ASSIGNABLE_OR_CONVERTIBLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_ASSIGNABLE_OR_CONVERTIBLE_HPP_

#include <type_traits>
#include "is_assignable.hpp"
#include "bool_constant.hpp"

namespace etude {

  template<class T, class U>
  struct is_assignable_or_convertible :
    etude::bool_constant<
      std::is_convertible<U, T>::value || etude::is_assignable<T, U>::value
    >::type
  {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_ASSIGNABLE_OR_CONVERTIBLE_HPP_
