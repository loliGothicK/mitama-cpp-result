//
//  is_assignable :
//    std::is_assignable の改良版
//    
//    etude::is_assignable<T, U> は、
//    型 T が参照でない場合は std::is_assignable<T&, U>::type を継承し、
//    型 T が参照の場合は std::false_type を継承します。
//
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_ASSIGNABLE_HPP_
#define ETUDE_TYPES_INCLUDED_IS_ASSIGNABLE_HPP_

#include <type_traits>
#include <utility>

namespace etude {

  // 実装クラス（ gcc4.5.0 だと std::is_assignable は未実装）
  template<class T, class U, class = void>
  class is_assignable_
  {
    template< class T_ = T&,
      class = decltype( std::declval<T_>() = std::declval<U>() )
    >
    static std::true_type test_( int );
    
    static std::false_type test_( ... );
    
   public:
    typedef decltype( test_(0) ) type;
    
  };
  
  // T がオブジェクトでない場合（参照を含む）は無条件で false
  template<class T, class U>
  struct is_assignable_<T, U,
    typename std::enable_if<
      !std::is_object<T>::value
    >::type
  >
  {
    typedef std::false_type type;
  };

  // 実装クラスに転送
  template<class T, class U>
  struct is_assignable
    : is_assignable_<T, U>::type {};

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_ASSIGNABLE_HPP_
