//
//  identity:
//    渡された型を返すメタ関数
// 
//    渡された型をそのまま type に定義します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IDENTITY_HPP_
#define ETUDE_TYPES_INCLUDED_IDENTITY_HPP_

namespace etude {
  
  template<class T>
  struct identity {
    typedef T type;
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IDENTITY_HPP_
