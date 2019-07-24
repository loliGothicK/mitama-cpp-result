//
//  wrapper:
//    値に対する単純なラッパークラス
// 
//    ADL ガードを搭載した単純なラッパークラスです。主に EBO をするために使います。
//    holder との違いは、構築が explicit でアクセスが implicit な点です。
//    
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_WRAPPER_HPP_
#define ETUDE_UTILITY_INCLUDED_WRAPPER_HPP_

#include <type_traits>
#include <utility>
#include <functional>
#include <boost/utility/addressof.hpp>
#include "uninitialized.hpp"
#include "operator_arrow.hpp"
#include "../types/is_derivable.hpp"
#include "../types/decay_and_strip.hpp"
#include "../types/is_constructible.hpp"

namespace etude {
 namespace wrapper_ { // ADL 回避
 
  template<class T, class = void>
  struct wrapper;
  
  // for scalar types
  template<class T>
  struct wrapper< T,
    typename std::enable_if< std::is_scalar<T>::value >::type
  >
  {
    // construct
    explicit wrapper( T x_ = T() )
      : x( x_ ) {}
    explicit wrapper( etude::uninitialized_t ) {}
    
    // observe
    operator T &      ()       { return x; }
    operator T const& () const { return x; }
    
    // operator->
    template<class U = T,
      class Result = decltype( etude::operator_arrow( U() ) )
    >
    T operator->() const { return x; }
    
   private:
    T x;
    
  };
  
  // for derivable types
  template<class T>
  struct wrapper< T,
    typename std::enable_if< etude::is_derivable<T>::value >::type
  >
    : T
  {
    // 構築のみ。 explicit
    template< class U = T,
      class = typename std::enable_if<
        etude::is_constructible<T, U&&>::value
      >::type
    >
    explicit wrapper( T && x )
      : T( std::forward<T>(x) ) {}
    
    template< class... Args,
      class = typename std::enable_if<
        etude::is_constructible<T, Args...>::value
      >::type
    >
    explicit wrapper( Args&&... args )
      : T( std::forward<Args>(args)... ) {}
    
    // gcc 4.5.0 には implicit move がない
    wrapper( wrapper const& ) = default;
    wrapper( wrapper &&     ) = default;
    
  };
  
  // for array_types
  template<class T, std::size_t N>
  struct wrapper<T[N]>
  {
    typedef T array_type[N];
    
    T elems[N];
    
    // コンストラクタは無し
    
    // 要素取得
    operator array_type      & ()       { return elems; }
    operator array_type const& () const { return elems; }
    
  };
  
  // for references
  template<class T>
  struct wrapper< T,
    typename std::enable_if<
      std::is_reference<T>::value
    >::type
  >
    : std::reference_wrapper<typename std::remove_reference<T>::type>
  {
    typedef std::reference_wrapper<typename std::remove_reference<T>::type> base_;
    
    explicit wrapper( T && x )
      : base_( x ) {}
    
    // lvalue reference の場合には rvalue を束縛できないように
    template< class U = T,
      class = typename std::enable_if< std::is_lvalue_reference<U>::value >::type
    >
    explicit wrapper( typename std::remove_reference<T>::type && x ) = delete;
    
    // 参照取得
    operator T&() const { return base_::get(); }
    // operator T&& () const && { return std::forward<T>(base_::get()); }
    
    // operator->
    template< class U = T&,
      class Result = decltype( etude::operator_arrow( std::declval<U>() ) )
    >
    Result operator->() const { return etude::operator_arrow( base_::get() ); }
    
  };
  
  
  // free funcions
  
  // wrapping
  template< class T,
    class U = typename decay_and_strip<T>::type
  >
  inline wrapper<U> wrap( T && x ) {
    return wrapper<U>( std::forward<T>(x) );
  }
  
  // unwrapping
  template<class T>
  inline T& unwrap( wrapper<T>& x ) {
    return x;
  }
  template<class T>
  inline T const& unwrap( wrapper<T> const& x ) {
    return x;
  }
  template<class T>
  inline T&& unwrap( wrapper<T> && x ) {
    return std::forward<T>( static_cast<T&>(x) );
  }
  
  // unwrap は wrapper を継承したクラスの場合には呼べないように
  template< class T >
  void unwrap( T const& ) = delete;
  
 }  // namespace wrapper_;
 using namespace wrapper_;
}

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_WRAPPER_HPP_
