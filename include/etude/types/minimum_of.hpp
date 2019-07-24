//
//  minimum_of :
//    与えられた引数のうち最小値を求めるメタ関数
//    
//    minimum_of<T, Xs...> は、 sizeof...(Xs) != 0 ならば、その時に限り
//    std::integral_constant<T, X> を継承します。
//    ただし X は、 T 型の値 Xs... のうちで最小の要素です。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_MINIMUM_OF_HPP_
#define ETUDE_TYPES_INCLUDED_MINIMUM_OF_HPP_

#include <type_traits>

namespace etude {

  template<class T, T... Xs>
  class minimum_of {};
  
  template<class T, T X>
  struct minimum_of<T, X>
    : std::integral_constant<T, X> {};
  
  template<class T, T X, T Y>
  struct minimum_of<T, X, Y>
    : std::integral_constant<T, ( Y < X ? Y : X )> {};
  
  template<class T, T X, T... Ys>
  struct minimum_of<T, X, Ys...>
    : minimum_of<T, X, minimum_of<T, Ys...>::value>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_MINIMUM_OF_HPP_
