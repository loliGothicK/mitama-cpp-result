//
//  any_of_c :
//    与えられた引数のうち一つでも true があれば true_type になるメタ関数
//    
//    any_of_c<Conds...> は、 bool 定数 Conds... が全て偽の場合、
//    または sizeof...(Conds) == 0 の場合には std::false_type を、
//    そうでなければ std::true_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_ANY_OF_C_HPP_
#define ETUDE_TYPES_INCLUDED_ANY_OF_C_HPP_

#include <type_traits>

namespace etude {

  template<bool... Conds>
  class any_of_c;
  
  template<>
  struct any_of_c<>
    : std::false_type {};

  template<bool... Conds>
  struct any_of_c<true, Conds...>
    : std::true_type {};
  
  template<bool... Conds>
  struct any_of_c<false, Conds...>
    : any_of_c<Conds...>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_ANY_OF_C_HPP_
