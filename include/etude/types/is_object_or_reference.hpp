//
//  is_object_or_reference :
//    オブジェクトまたは参照の場合には true になるメタ関数
//    
//    etude::is_object_or_reference<T> は、 T が object type または参照の場合には
//    std::true_type を、そうでない場合には std::false_type を継承します。
//
//    [ note:
//        つまり、 T が void または関数の場合以外は true_type を継承します。
//    ]
//
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_OBJECT_OR_REFERENCE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_OBJECT_OR_REFERENCE_HPP_

#include "bool_constant.hpp"
#include <type_traits>

namespace etude {

  // そのまんまの定義
  template<class T>
  struct is_object_or_reference :
    etude::bool_constant<
      std::is_object<T>::value || std::is_reference<T>::value
    >::type
  {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_OBJECT_OR_REFERENCE_HPP_
