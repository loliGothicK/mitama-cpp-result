//
//  types:
//    複数の型に対する単純な type envelope (＋α)
// 
//    様々な局面で型の代わりとして使うタグです。
//    タプルのアンパックに使う indices と違って、それだけだと効果は薄いですが、
//    代わりに etude::is_convertible など対する特殊引数としても使えるようになっています。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TYPES_HPP_
#define ETUDE_TYPES_INCLUDED_TYPES_HPP_

#include <cstddef>  // for std::size_t

namespace etude {
  
  // シンプルな type envelope
  // っていうか boost::type を使えばいいんだけど、 #include すら面倒なので。
  template<class T>
  struct type {};
  
  // 複数の型に対する type envelope
  template<class... Types>
  struct types
  {
    typedef types type;
    static std::size_t const size = sizeof...(Types);
    
    template<template <class...> class U>
    struct apply {
      typedef U<Types...> type;
    };
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TYPES_HPP_
