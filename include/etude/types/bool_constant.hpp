//
//  bool_constant:
//    コンパイル時 bool 定数
// 
//    etude::size_constant<b> は std::integral_constant<bool, b> を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_BOOL_CONSTANT_HPP_
#define ETUDE_TYPES_INCLUDED_BOOL_CONSTANT_HPP_

#include <type_traits>

namespace etude {
  
  template<bool b>
  struct bool_constant
    : std::integral_constant<bool, b> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_BOOL_CONSTANT_HPP_
