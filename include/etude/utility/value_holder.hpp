//
//  value_holder:
//    タプルを unpack しながら構築できるラッパークラス
// 
//    単独では特に意味はありませんが、メンバとして使うときに便利です。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_VALUE_HOLDER_HPP_
#define ETUDE_UTILITY_INCLUDED_VALUE_HOLDER_HPP_

#include <type_traits>
#include <utility>
#include <boost/utility/addressof.hpp>

#include "holder.hpp"
#include "emplace_construct.hpp"
#include "unpack_construct.hpp"
#include "../unpacked_tuple.hpp"
#include "../unpack.hpp"
#include "../functional/pack.hpp"
#include "../functional/group.hpp"

#include "../types/indices.hpp"
#include "tuple_get.hpp"
#include "../types/tuple_types.hpp"
#include "../types/tuple_indices.hpp"
#include "../types/is_constructible.hpp"
#include "../types/is_unpack_constructible.hpp"

namespace etude {
 namespace value_holder_ { // ADL 回避
 
  // 直接 etude::holder を使うのはアレなので
  // 実装用クラスを持ってくる
  using etude::holder_::holder_;
  
  // タプルの unpack をするにはもう一段実装用クラスをはさむ必要あり
  template<class T, class Tag>
  class value_holder_
    : private holder_<T, Tag>
  {
    typedef holder_<T, Tag> base;
    
   public:
    using base::get;
    using base::move;
    
    // デフォルト構築
    value_holder_() = default;
    
    // pack されてない引数から構築
    template<class... Args>
    value_holder_( emplace_construct_t, Args&&... args )
      : base( std::forward<Args>(args)... ) {}
    
    // pack された引数から構築
    template< class Tuple, std::size_t... Indices >
    value_holder_( unpacked_tuple<Tuple, Indices...> t )
      : base ( etude::move<Indices>(t)... )
    {
      (void)t;  // unused variable 警告避け（ Tuple が空の場合に）
    }
    
    // gcc4.5.0 では implicit move が実装されていないので
    value_holder_( value_holder_ const& ) = default;
    value_holder_( value_holder_ && )     = default;
    
    // swap
    void swap( value_holder_& x ) {
      base::swap( static_cast<base&>(x) );
    }
    
  };
  
  // 本体
  template<class T, class Tag = void>
  class value_holder
    : private value_holder_<typename std::remove_const<T>::type, Tag>
  {
    typedef typename std::remove_const<T>::type T_;
    typedef value_holder_<T_, Tag> base;
    typedef value_holder<T, Tag> self_type;
    
   public:
    typedef T_   value_type;
    typedef T  element_type;
    
    typedef T &            reference;
    typedef T const& const_reference;
    typedef T_ &&   rvalue_reference;
    
    typedef typename std::add_pointer<T>::type             pointer;
    typedef typename std::add_pointer<T const>::type const_pointer;
    
    // デフォルト構築
    value_holder() = default;
    
    // T からの構築
    template<class U = T_&&,
      class = typename std::enable_if<
        etude::is_constructible<T, U>::value
      >::type
    >
    value_holder( T_ && src )
      : base( emplace_construct, std::forward<T_>(src) ) {}
    // 型変換構築（ const T& も含む）
    template<class U,
      class = typename std::enable_if<
        !std::is_reference<T>::value && // 参照の場合は上の ctor を使わせる
        std::is_convertible< U, holder<T> >::value
      >::type
    >
    value_holder( U && src )
      : base( emplace_construct, std::forward<U>(src) ) {}
    // lvalue reference に rvalue reference を束縛させようとしている場合は delete する
    template<class U = T_>
    value_holder (
      typename std::enable_if<
        std::is_lvalue_reference<U>::value,
        typename std::remove_reference<U>::type
      >::type &&
    ) = delete;
    
    // pack されてない引数から構築
    template<class... Args,
      class = typename std::enable_if<
        etude::is_constructible<holder<T>, Args...>::value
      >::type
    >
    value_holder( emplace_construct_t, Args&&... args )
      : base( emplace_construct, std::forward<Args>(args)... ) {}
    
    // pack された引数から構築
    template<class Tuple,
      class = typename std::enable_if<
        etude::is_unpack_constructible< holder<T>, Tuple >::value
      >::type
    >
    value_holder( unpack_construct_t, Tuple && t )
      : base( etude::unpack( std::forward<Tuple>(t) ) ) {}
    
    // unpack された引数から構築
    template< class... Args,
      class = typename std::enable_if<
        etude::is_unpack_constructible<
          holder<T>,
          decltype( etude::pack( std::declval<Args>()... ) )
        >::value
      >::type
    >
    explicit value_holder( Args&&... args )
      : base( etude::group( std::forward<Args>(args)... ) ) {}
    
    // gcc4.5.0 では implicit move が（ｒｙ
    value_holder( self_type const& ) = default;
    value_holder( self_type && )     = default;
    
    // 型変換コンストラクタ
    // copy
    template<class U, class Tag_,
      class U_ = typename std::remove_const<U>::type,
      class = typename std::enable_if<
        std::is_convertible<U_, T>::value
      >::type
    >
    value_holder( value_holder<U, Tag_> const& src )
      : base( emplace_construct, src.get() ) {}
    // move
    template<class U, class Tag_,
      class U_ = typename std::remove_const<U>::type,
      class = typename std::enable_if<
        std::is_convertible<U_, T>::value
      >::type
    >
    value_holder( value_holder<U, Tag_> && src )
      : base( emplace_construct, src.move() ) {}
    
    
    // 再代入
    
    // T が参照の場合は etude::is_assignable は常時 false なので、
    // 暗黙に定義されたコピー代入演算子が使われる
    
    // 単純再代入
    template< class U = T_,
      class = typename std::enable_if<
        etude::is_assignable<T_, U>::value
      >::type
    >
    value_holder& operator=( T_ && x ) {
      base::get() = std::forward<T_>(x);
      return *this;
    }
    template< class U,
      class = typename std::enable_if<
        etude::is_assignable<T_, U>::value
      >::type
    >
    value_holder& operator=( U && x ) {
      base::get() = std::forward<U>(x);
      return *this;
    }
    
    // copy
    template<class U, class Tag_,
      class = typename std::enable_if<
        etude::is_assignable<T_, U const&>::value
      >::type
    >
    value_holder& operator=( value_holder<U, Tag_> const& rhs ) {
      base::get() = rhs.get();
      return *this;
    }
    // move
    template<class U, class Tag_,
      class U_ = typename std::remove_const<U>::type,
      class = typename std::enable_if<
        etude::is_assignable<T_, U_>::value
      >::type
    >
    value_holder& operator=( value_holder<U, Tag_> && rhs ) {
      base::get() = rhs.move();
      return *this;
    }
    
    
    // get/move
    
    // get は T が const U の場合に対処するため明示的に指定する
    T &      get()       { return base::get(); }
    T const& get() const { return base::get(); }
    // move はそのまま
    using base::move;
    
    // get_ptr
    pointer       get_ptr()       { return boost::addressof( get() ); }
    const_pointer get_ptr() const { return boost::addressof( get() ); }
    
    // operator*
    friend T &      operator*( value_holder &      x ){ return x.get(); }
    friend T const& operator*( value_holder const& x ){ return x.get(); }
    friend T_ &&    operator*( value_holder &&     x ){ return x.move(); }
    
    // operator->
    pointer       operator->()       { return get_ptr(); }
    const_pointer operator->() const { return get_ptr(); }
    
    
    // swap
    void swap( self_type& x ) {
      base::swap( static_cast<base&>(x) );
    }
    
  };
  
  
  // 自由関数版 get
  template<class T, class Tag>
  inline T& get( value_holder<T, Tag>& x ) {
    return x.get();
  }
  template<class T, class Tag>
  inline T const& get( value_holder<T, Tag> const& x ) {
    return x.get();
  }
  // move 版
  template<class T, class Tag>
  inline typename std::remove_const<T>::type && get( value_holder<T, Tag> && x ) {
    return x.move();
  }
  
  // ポインタ版 get
  template<class T, class Tag>
  inline auto get( value_holder<T, Tag>* p )
    -> decltype( p->get_ptr() )
  {
    return p ? p->get_ptr() : 0;
  }
  template<class T, class Tag>
  inline auto get( value_holder<T, Tag> const* p )
    -> decltype( p->get_ptr() )
  {
    return p ? p->get_ptr() : 0;
  }
  
  // 自由関数版 get_ptr
  template<class T, class Tag>
  inline auto get_pointer( value_holder<T, Tag>& x )
    -> decltype( x.get_ptr() )
  {
    return x.get_ptr();
  }
  template<class T, class Tag>
  inline auto get_pointer( value_holder<T, Tag> const& x )
    -> decltype( x.get_ptr() )
  {
    return x.get_ptr();
  }
  
  
  // 自由関数版 swap
  template<class T, class Tag>
  inline void swap( value_holder<T, Tag>& one, value_holder<T, Tag>& another ) {
    one.swap( another );
  }
 
 }  // namespace value_holder_
 using namespace value_holder_;
}

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_VALUE_HOLDER_HPP_
