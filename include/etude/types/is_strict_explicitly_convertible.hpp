//
//  is_strict_explicitly_convertible :
//    explicit 修飾されたコンストラクタが存在するなら true になるメタ関数
//    
//    is_strict_explicitly_convertible<From, To> は、
//    std::is_explicitly_convertible<From, To>::value が true かつ
//    std::is_convertible<From, To>::value が false ならば std::true_type を、
//    そうでなければ std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_STRICT_EXPLICITLY_CONVERTIBLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_STRICT_EXPLICITLY_CONVERTIBLE_HPP_

#include <type_traits>
#include "bool_constant.hpp"
#include "is_constructible.hpp"

namespace etude {

  template<class From, class To>
  struct is_strict_explicitly_convertible :
    etude::bool_constant<
      etude::is_constructible<To, From>::value &&
      !std::is_convertible<From, To>::value
    >::type
  {
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_STRICT_EXPLICITLY_CONVERTIBLE_HPP_
