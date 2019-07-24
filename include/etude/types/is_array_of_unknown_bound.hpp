//
//  is_array_of_unknown_bound :
//    要素数不明の配列ならば true_type になるメタ関数
//    
//    is_array_of_unknown_bound<T> は、 T が要素数不明の配列の場合には
//    std::true_type を、そうでない場合には std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_ARRAY_OF_UNKNOWN_BOUND_HPP_
#define ETUDE_TYPES_INCLUDED_IS_ARRAY_OF_UNKNOWN_BOUND_HPP_

#include <type_traits>

namespace etude {

  template<class T>
  struct is_array_of_unknown_bound
    : std::false_type {};
  
  template<class T>
  struct is_array_of_unknown_bound<T[]>
    : std::true_type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_ARRAY_OF_UNKNOWN_BOUND_HPP_
