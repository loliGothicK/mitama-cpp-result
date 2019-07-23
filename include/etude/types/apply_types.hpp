//
//  apply_types:
//    types の保持する型をテンプレートに適用
// 
//    typename apply_types<Template, Types>::type は、 typename Types::type が
//    etude::types<Ts...> である場合には Template<Ts...> に定義され、
//    それ以外の場合には定義されません。
//    
//    なお、 typename apply_types<Template, Types>::type が定義される場合、その型は
//      typename Types::type::template apply<Template>::type
//    と等しくなります。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_APPLY_TYPES_HPP_
#define ETUDE_TYPES_INCLUDED_APPLY_TYPES_HPP_

#include "types.hpp"
#include "get_type_or.hpp"

namespace etude {
  
  template< template<class...> class Template, class Types >
  struct apply_types_ {};
  
  template< template<class...> class Template, class... Types >
  struct apply_types_< Template, etude::types<Types...> >
  {
    typedef Template<Types...> type;
  };
  
  template< template<class...> class Template, class Types >
  struct apply_types
    : apply_types_<Template, typename get_type_or<Types>::type> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_APPLY_TYPES_HPP_
