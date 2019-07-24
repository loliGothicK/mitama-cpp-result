//
//  has_common_type:
//    std::common_type が有るか否か
// 
//    etude::has_common_type<Types...> は、
//    std::common_type<Types...>::type がエラーなしに定義される場合には
//    std::true_type を、エラーになる場合には std::false_type を継承します。
// 
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_HAS_COMMON_TYPE_HPP_
#define ETUDE_TYPES_INCLUDED_HAS_COMMON_TYPE_HPP_

#include <type_traits>
#include <utility>
#include "is_void_pointer.hpp"

namespace etude {
  
  // ゼロ引数
  template<class... Types>
  struct has_common_type
    : std::false_type {};
  
  
  // １引数
  template<class T>
  struct has_common_type<T>
    : std::true_type {};
  
  
  // ２引数
  template<class T, class U>
  class has_common_type_impl_
  {
    template< class T_ = T, class U_ = U,
      class R = decltype( true ? std::declval<T_>() : std::declval<U_>() ),
      class = typename std::enable_if<
        !(
          // なんか無関係なポインタ同士だと void* にフォールバックしてしまう場合がある
          is_void_pointer<R>::value && !(
            is_void_pointer<typename std::decay<T_>::type>::value ||
            is_void_pointer<typename std::decay<U_>::type>::value
          )
        ) && (
          // なんか T** と U** から void** にフォールバックしてしまう場合もある
          std::is_convertible<T_, R>::value &&
          std::is_convertible<U_, R>::value
        )
      >::type
    >
    static std::true_type test_( int );
    
    static std::false_type test_( ... );
    
   public:
    typedef decltype( test_(0) ) type;
    
  };
  template<class T, class U>
  struct has_common_type<T, U>
    : has_common_type_impl_<T, U>::type {};
  
  
  // ３引数以上
  template<class Void, class T, class U, class... Types>
  struct has_common_type_
    : std::false_type {};
  
  template<class T, class U, class... Types>
  struct has_common_type_<
    typename std::enable_if<
      etude::has_common_type<T, U>::value
    >::type,
    T, U, Types...
  >
    : etude::has_common_type< typename std::common_type<T, U>::type, Types... >::type {};
  
  template<class T, class U, class... Types>
  struct has_common_type<T, U, Types...>
    : has_common_type_<void, T, U, Types...>::type {};


} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_HAS_COMMON_TYPE_HPP_
