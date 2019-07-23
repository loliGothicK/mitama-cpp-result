//
//  tuple_element :
//    std::tuple_element の改良版
//    
//    etude::tuple_element<I, Tuple>::type は、
//    Tuple が参照でないの場合は std::tuple_element<I, Tuple>::type と等しく、
//    Tuple が参照の場合は std::get<I>( std::declval<Tuple>() ) の戻り値の型になります。
//    N3225 で rvalue に対する get が定義されたため、
//    それも正しく扱える（ rvalue-ref ならば rvalue-ref になる）ようになってます。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_TUPLE_ELEMENT_HPP_
#define ETUDE_TYPES_INCLUDED_TUPLE_ELEMENT_HPP_

#include <tuple>
#include <type_traits>
#include "tuple_size.hpp"

namespace etude {

  // 前方宣言
  template<std::size_t I, class Tuple>
  struct tuple_element;

  // 実装
  template<std::size_t I, class Tuple>
  struct tuple_element_impl_
  {
    typedef typename std::tuple_element<I, Tuple>::type type;
    
    typedef typename std::add_lvalue_reference<type>::type             reference;
    typedef typename std::add_lvalue_reference<type const>::type const_reference;
    
    static reference get( Tuple& x ) {
      using std::get;
      return get<I>(x);
    }
    
    static const_reference get( Tuple const& x ) {
      using std::get;
      return get<I>(x);
    }
    
  };
  
  // cv 付き
  #define ETUDE_TUPLE_ELEMENT_GEN_( cv )                      \
    template< std::size_t I, class Tuple >                    \
    struct tuple_element_impl_< I, Tuple cv >                 \
    {                                                         \
      typedef typename tuple_element<I, Tuple>::type cv type; \
                                                              \
      static type& get( Tuple cv& x ) {                       \
        return tuple_element<I, Tuple>::get(x);               \
      }                                                       \
                                                              \
    };                                                        \
    /* ETUDE_TUPLE_ELEMENT_GEN_( cv ) */
    
    ETUDE_TUPLE_ELEMENT_GEN_( const )
    ETUDE_TUPLE_ELEMENT_GEN_( volatile )
    ETUDE_TUPLE_ELEMENT_GEN_( const volatile )
  
  #undef ETUDE_TUPLE_ELEMENT_GEN_
  
  // 参照
  template< std::size_t I, class Tuple >
  struct tuple_element_impl_< I, Tuple& >
  {
    typedef typename std::add_lvalue_reference<
      typename tuple_element<I, Tuple>::type
    >::type type;
    
    static type get( Tuple& x ) {
      return tuple_element<I, Tuple>::get(x);
    }
    
  };
  template< std::size_t I, class Tuple >
  struct tuple_element_impl_< I, Tuple&& >
  {
    typedef typename std::add_rvalue_reference<
      typename tuple_element<I, Tuple>::type
    >::type type;
    
    static type get( Tuple && x ) {
      return std::forward<type>( tuple_element<I, Tuple>::get(x) );
    }
    
  };

  template<std::size_t I, class Tuple, class = void>
  struct tuple_element_{};
  
  template<std::size_t I, class Tuple>
  struct tuple_element_< I, Tuple,
    typename std::enable_if<
      ( I < etude::tuple_size<Tuple>::value )
    >::type
  >
    : tuple_element_impl_<I, Tuple> {};
  
  // 実装
  template<std::size_t I, class Tuple>
  struct tuple_element
    : tuple_element_<I, Tuple> {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_TUPLE_ELEMENT_HPP_
