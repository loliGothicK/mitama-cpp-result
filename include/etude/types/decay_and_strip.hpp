//
//  decay_and_strip :
//    decay して reference_wrapper を取り除く
//    
//    型 T に対し、 decay_and_strip<T>::type は、
//    型 U を std::decay<T>::type としたとき、
//      （これにより lvalue は rvalue に、配列や関数はポインタになります）
//    U が std::reference_wrapper<X> ならば X& に定義され、
//    そうでなければ U に定義されます。
//    
//    この操作は std::make_tuple において各引数に対して行われる操作と同じです。
//
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
//  このヘッダは gcc 4.5.0 の <tuple> 実装を参考にしています。
//
#ifndef ETUDE_TYPES_INCLUDED_DECAY_AND_STRIP_HPP_
#define ETUDE_TYPES_INCLUDED_DECAY_AND_STRIP_HPP_

#include <type_traits>  // for std::decay
#include <functional>   // for std::reference_wrapper

namespace etude {

  // for implementation
  // strip reference_wrapper
  template<class U>
  struct strip_reference_wrapper {
    typedef U type;
  };
  template<class X>
  struct strip_reference_wrapper<std::reference_wrapper<X>> {
    typedef X& type;
  };
  
  // 本体
  template<class T>
  struct decay_and_strip
    : strip_reference_wrapper<typename std::decay<T>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_DECAY_AND_STRIP_HPP_
