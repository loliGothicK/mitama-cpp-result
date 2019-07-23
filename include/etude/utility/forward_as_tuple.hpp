//
//  forward_as_tuple:
//    std::forward_as_tuple と同じ関数
// 
//    std::forward_as_tuple がまだ gcc に実装されてないので、実装されるまでのつなぎです。
//    
//    与えられた引数は一時オブジェクトを含め、参照により「そのままに」束縛されます。
//    この関数の呼び出した結果は、他の関数に転送されることを想定しています。
//  
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_FORWARD_AS_TUPLE_HPP_
#define ETUDE_UTILITY_INCLUDED_FORWARD_AS_TUPLE_HPP_

#include <tuple>
#include <utility>

namespace etude {

  template<class... Types>
  inline std::tuple<Types&&...> forward_as_tuple( Types&&... args ) {
    return std::tuple<Types&&...>( std::forward<Types>(args)... );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_FORWARD_AS_TUPLE_HPP_
