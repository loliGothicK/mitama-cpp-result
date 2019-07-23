//
//  nth_type:
//    与えられた型リストのうち、 N+1 番目の引数を返す
// 
//    typename etude::nth_type<N, Ts...>::type は、
//    N <  sizeof...(Ts) の場合には Ts... のうち N 番目の型（ N は 0 ベース）に定義され、
//    N >= sizeof...(Ts) の場合には定義されません。
//    
//    参考：
//    これは typename std::tuple_element<N, std::tuple<Ts...>>::type と同じ型になります。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_NTH_TYPE_HPP_
#define ETUDE_TYPES_INCLUDED_NTH_TYPE_HPP_

#include <cstddef>  // for std::size_t

namespace etude {
  
  template<std::size_t N, class... Ts>
  class nth_type;
  
  // Ts... が無ければ、どんな N に対しても nth_type は存在しない
  template<std::size_t N>
  class nth_type<N> {};
  
  // N が 0 の場合には最初の要素
  template<class T, class... Ts>
  struct nth_type<0, T, Ts...>
  {
    typedef T type;
  };
  
  // それ以外の場合には一つずらす
  template<std::size_t N, class T, class... Ts>
  struct nth_type<N, T, Ts...>
    : nth_type<N-1, Ts...> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_NTH_TYPE_HPP_
