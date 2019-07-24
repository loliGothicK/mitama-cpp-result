//
//  empty_base:
//    様々な用途に使う空の基底クラス
// 
//    empty_base<T> は空のクラスです。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_EMPTY_BASE_HPP_
#define ETUDE_UTILITY_INCLUDED_EMPTY_BASE_HPP_

namespace etude {
 namespace empty_base_ {
  
  template<class T = void>
  struct empty_base {};
  
 }
 using namespace empty_base_;
}

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_EMPTY_BASE_HPP_
