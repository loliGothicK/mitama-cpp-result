//
//  uninitialized:
//    中身を明示的に初期化しない、という旨を示すためのクラス
// 
//    主に storage で使います。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_UNINITIALIZED_HPP_
#define ETUDE_UTILITY_INCLUDED_UNINITIALIZED_HPP_

namespace etude {
 
  struct uninitialized_t {};
  
  namespace {
    uninitialized_t const uninitialized = {};
  }
  
}

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_UNINITIALIZED_HPP_
