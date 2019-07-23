//
//  get_type_or:
//    メタ関数ならば型を取り出すメタ関数
// 
//    typename get_type_or<T, Default>::type は、
//    typename T::type が存在する場合は typename T::type に、
//    存在しない場合には Default に定義されます。
//    
//    Default を省略した場合は void になります。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_GET_TYPE_OR_HPP_
#define ETUDE_TYPES_INCLUDED_GET_TYPE_OR_HPP_

#include "make_void.hpp"

namespace etude {
  
  // T::type が存在しない場合
  template< class T, class Default, class = void >
  struct get_type_or_
  {
    typedef Default type;
  };
  // T::type が存在する場合
  template< class T, class Default >
  struct get_type_or_< T, Default,
    typename etude::make_void<typename T::type>::type
  >
  {
    typedef typename T::type type;
  };
  
  // 本体
  template< class T, class Default = void >
  struct get_type_or
  {
    typedef typename get_type_or_<T, Default>::type type;
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_GET_TYPE_OR_HPP_
