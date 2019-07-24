//
//  size_constant:
//    コンパイル時サイズ型定数
// 
//    etude::size_constant<N> は std::integral_constant<std::size_t, N> を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_SIZE_CONSTANT_HPP_
#define ETUDE_TYPES_INCLUDED_SIZE_CONSTANT_HPP_

#include <type_traits>

namespace etude {
  
  template<std::size_t N>
  struct size_constant
    : std::integral_constant<std::size_t, N> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_SIZE_CONSTANT_HPP_
