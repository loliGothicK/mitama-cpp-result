//
//  unpack_construct:
//    tuple から構築する、という旨を明示するためのタグ用クラス
// 
//    klass x( unpack_construct, some_tuple );
//    
//    のように使うことを想定しています。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_UNPACK_CONSTRUCT_HPP_
#define ETUDE_UTILITY_INCLUDED_UNPACK_CONSTRUCT_HPP_

namespace etude {
 
  struct unpack_construct_t {};
  
  namespace {
    unpack_construct_t const unpack_construct = {};
  }
  
}

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_UNPACK_CONSTRUCT_HPP_
