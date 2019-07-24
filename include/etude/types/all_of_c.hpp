//
//  all_of_c :
//    与えられた引数が全て真なら true_type になるメタ関数
//    
//    all_of_c<Conds...> は、 bool 定数 Conds... が全て真の場合、
//    または sizeof...(Conds) == 0 の場合には std::true_type を、
//    そうでなければ std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_ALL_OF_C_HPP_
#define ETUDE_TYPES_INCLUDED_ALL_OF_C_HPP_

#include <type_traits>

namespace etude {

  template<bool... Conds>
  struct all_of_c;
  
  template<>
  struct all_of_c<>
    : std::true_type {};
  
  template<bool... Conds>
  struct all_of_c<true, Conds...>
    : all_of_c<Conds...>::type {};

  template<bool... Conds>
  struct all_of_c<false, Conds...>
    : std::false_type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_ALL_OF_C_HPP_
