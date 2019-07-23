//
//  make_void:
//    常に void を返すメタ関数
// 
//    渡された型を無視して void に定義されます。
//    主にクラスに対する SFINAE に使います。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_MAKE_VOID_HPP_
#define ETUDE_TYPES_INCLUDED_MAKE_VOID_HPP_

namespace etude {
  
  template<class... Types>
  struct make_void {
    typedef void type;
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_MAKE_VOID_HPP_
