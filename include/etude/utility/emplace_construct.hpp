//
//  emplace_construct:
//    引数リストから直接構築する、という旨を明示するためのタグ用クラス
// 
//    主に value_holder で使います。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_EMPLACE_CONSTRUCT_HPP_
#define ETUDE_UTILITY_INCLUDED_EMPLACE_CONSTRUCT_HPP_

namespace etude {
 
  struct emplace_construct_t {};
  
  namespace {
    emplace_construct_t const emplace_construct = {};
  }
  
}

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_EMPLACE_CONSTRUCT_HPP_
