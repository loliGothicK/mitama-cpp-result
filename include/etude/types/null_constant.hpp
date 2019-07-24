//
//  null_constant:
//    値を持たない integral_constant
// 
//    渡された型の「何も値を保持しない」コンパイル時定数を定義します。
//    主に std::integral_constant を置きたいけど妥当な定数がない場所に使います。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_NULL_CONSTANT_HPP_
#define ETUDE_TYPES_INCLUDED_NULL_CONSTANT_HPP_

namespace etude {
  
  template<class T>
  struct null_constant
  {
    typedef null_constant<T> type;
    typedef T value_type;
  };
  
  typedef null_constant<void> void_type;

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_NULL_CONSTANT_HPP_
