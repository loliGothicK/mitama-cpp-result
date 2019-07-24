//
//  is_function_pointer :
//    関数ポインタならば true_type になるメタ関数
//    
//    is_function_pointer<T> は、 T が（メンバ関数ポインタを除く）関数ポインタならば
//    std::true_type を、そうでない場合には std::false_type を継承します。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_FUNCTION_POINTER_HPP_
#define ETUDE_TYPES_INCLUDED_IS_FUNCTION_POINTER_HPP_

#include "bool_constant.hpp"
#include <type_traits>

namespace etude {

  template<class T>
  struct is_function_pointer :
    etude::bool_constant<
      std::is_pointer<T>::value &&
      std::is_function<typename std::remove_pointer<T>::type>::value
    >::type
  {
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_FUNCTION_POINTER_HPP_
