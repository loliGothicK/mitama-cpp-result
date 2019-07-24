//
//  indices:
//    インデックスのリストに対する type envelope
// 
//    複数の std::size_t のコンパイル時定数をテンプレート引数に持ったクラスです。
//    主に関数などのテンプレートにインデックスのリストを渡したい場合に、
//    type envelope として使う感じになります。
//    
//    これにより、
//      std::get<Indices>( some_tuple )...
//    のようにして tuple を unpack する、といったことが出来るようになります。
//    
//    この用途に使う場合、 0 .. N-1 のインデックスリストがあると特に便利なので、
//    そのようなリストを生成するメタ関数 make_indices も用意。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
//  このヘッダは http://www.rhinocerus.net/forum/language-c-moderated/
//    439657-c-0x-tuple-unpacking-arguments-function-call.html#post2038703
//  を参考にしています。
//
#ifndef ETUDE_TYPES_INCLUDED_INDICES_HPP_
#define ETUDE_TYPES_INCLUDED_INDICES_HPP_

#include <cstddef>  // for std::size_t

namespace etude {
  
  // simple type-envelope for indices
  template<std::size_t... Indices>
  struct indices {
    typedef indices type;
    static std::size_t const size = sizeof...(Indices);
  };
  
  // make indices<0, 1, ... , N-1>
  
  // implementation classes
  template< class Indices >
  struct make_indices_impl_ {};
  
  template< std::size_t... Indices >
  struct make_indices_impl_< indices<Indices...> >
  {
    typedef indices<Indices..., sizeof...(Indices)> type;
  };
  
  
  template< int N,
    bool IsValid = ( 0 <= N && N < 1024 ) // 1024 : Max Template arguments
  >
  struct make_indices_ {};
  
  template<>
  struct make_indices_<0, true>
  {
    typedef indices<> type;
  };
  
  template< int N >
  struct make_indices_<N, true>
  {
    typedef typename make_indices_<N-1>::type init_type;
    typedef typename make_indices_impl_<init_type>::type type;
  };
  
  // 本体
  template<int N>
  struct make_indices
    : make_indices_<N>::type
  {
    static_assert( 0 <= N,   "make_indices : N must be greater than 0." );
    static_assert( N < 1024, "make_indices : N must be less than 1024." );
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_INDICES_HPP_
