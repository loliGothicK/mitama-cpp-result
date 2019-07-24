//
//  piecewise_construct:
//    std::piecewise_construct が出来るまでの「つなぎ」
// 
//    プリプロセッサで切り分けられるよう。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_PIECEWISE_CONSTRUCT_HPP_
#define ETUDE_UTILITY_INCLUDED_PIECEWISE_CONSTRUCT_HPP_


#if !defined(ETUDE_USING_STD_PIECEWISE_CONSTRUCT) \
  || defined(ETUDE_NO_STD_PIECEWISE_CONSTRUCT)

namespace etude {
 
  struct piecewise_construct_t {};
  
  namespace {
    piecewise_construct_t const piecewise_construct = {};
  }
  
}

#else   // #if !defined(ETUDE_USING_STD_PIECEWISE_CONSTRUCT) || defined(ETUDE_NO_STD_PIECEWISE_CONSTRUCT)

#include <utility>

namespace etude {
  using std::piecewise_construct;
  using std::piecewise_construct_t;
}

#endif  // #if !defined(ETUDE_USING_STD_PIECEWISE_CONSTRUCT) || defined(ETUDE_NO_STD_PIECEWISE_CONSTRUCT)


#endif  // #ifndef ETUDE_UTILITY_INCLUDED_PIECEWISE_CONSTRUCT_HPP_
