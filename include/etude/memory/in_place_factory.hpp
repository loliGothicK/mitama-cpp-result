//
//  in_place_factory:
//    Boost.InPlaceFactory の C++0x 拡張
// 
//    boost::in_place を variadic templates + perfect forward に対応させた版です。
//    普通の apply に加え、rvalue を move させる move_apply が追加されています。
//    本当は move_apply は rvalue references for *this を使って対応すべきなのでしょうが、
//    gcc 4.5.0 にはまだ実装されていないので、泣く泣く専用のメンバを用意しています。
//    ただ、 move 時は専用の関数を使う、というのだと扱いにくいので、
//    統一したアクセスを可能にするために、自由関数 apply_in_place も提供しています。
//    これは Boost の InPlaceFactory にも使えると嬉しいので、
//    ヘッダ apply_in_place.hpp に一般的に使える apply_in_place も用意されています。
//    
//  Copyright (C) 2010  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_MEMORY_INCLUDED_IN_PLACE_FACTORY_HPP_
#define ETUDE_MEMORY_INCLUDED_IN_PLACE_FACTORY_HPP_

#include <boost/utility/in_place_factory.hpp>

#include <utility>
#include <tuple>
#include <new>
#include <type_traits>

#include "is_in_place_factory.hpp"
#include "../types/indices.hpp"
#include "../types/types.hpp"
#include "../types/tuple_convertible.hpp"
#include "../types/decay_and_strip.hpp"
#include "../types/tuple_indices.hpp"
#include "../utility/tuple_get.hpp"

namespace etude {

  // class template in_place_factory
  
  // Variadic Templates を使って書かれた InPlaceFactory
  // rvalue-reference にも対応している
  template<class... Args>
  struct in_place_factory
    : boost::in_place_factory_base
  {
    // 引数パックとして使われる tuple
    typedef std::tuple<Args...> tuple_type;
    
    // 普通に構築
    template< class... Types,
      class = typename std::enable_if<
        etude::tuple_convertible<std::tuple<Types...>, tuple_type>::value
      >::type
    >
    explicit in_place_factory( Types&&... args )
      : x( std::forward<Types>(args)... ) {}
    
    // gcc 4.5.0 では implicit move は実装されていない
    in_place_factory( in_place_factory const& ) = default;
    in_place_factory( in_place_factory && ) = default;
    in_place_factory& operator=( in_place_factory const& ) = delete;
    
    // tuple からの構築
    in_place_factory( tuple_type const& t )
      : x( t ) {}
    in_place_factory( tuple_type && t )
      : x( std::move(t) ) {}
    
    // 型変換
    // きちんと変換できるもんだけ変換する
    // copy
    // （Types const&...）だと SFINAE のはずがエラーになるので Types... に。
    template<class... Types,
      class = typename std::enable_if<
        etude::tuple_convertible<std::tuple<Types...>, tuple_type>::value
      >::type
    >
    in_place_factory( in_place_factory<Types...> const& src )
      : x( src.get_tuple() ) {}
    // move
    template<class... Types,
      class = typename std::enable_if<
        etude::tuple_convertible<std::tuple<Types...>, tuple_type>::value
      >::type
    >
    in_place_factory( in_place_factory<Types...> && src )
      : x( src.move_tuple() ) {}
    
    // 中身の tuple を得る
    tuple_type const& get_tuple() const { return x; }
    tuple_type&& move_tuple() { return std::move(x); }
    
    
    // 適用
    template<class T>
    T* apply( void* addr ) const {
      return apply_<T>( addr, make_indices<sizeof...(Args)>() );
    }
    // move して適用
    template<class T>
    T* move_apply( void* addr ) {
      return move_apply_<T>( addr, make_indices<sizeof...(Args)>() );
    }
    
   private:
    tuple_type x;
    
    // 実装用
    template<class T, std::size_t... Indices>
    T* apply_( void* addr, indices<Indices...> ) const {
      return ::new(addr) T( std::get<Indices>(x)... );
    }
    template<class T, std::size_t... Indices>
    T* move_apply_( void* addr, indices<Indices...> ) {
      return ::new(addr) T( std::forward<Args>( std::get<Indices>(x) )... );
    }
  };
  
  
  // helper functions
  
  // get_tuple の自由関数版
  // こちらのほうが名前が統一されてるので、基本的にこっちを使うべき
  template<class... Args>
  inline auto get_tuple( in_place_factory<Args...> const& x )
    -> decltype( x.get_tuple() ) { return x.get_tuple(); }
  template<class... Args>
  inline auto get_tuple( in_place_factory<Args...> && x )
    -> decltype( x.move_tuple() ) { return x.move_tuple(); }
  
  // apply の自由関数版
  // 参照
  template<class T, class... Args>
  inline T* apply_in_place( in_place_factory<Args...>& x, void* addr ) {
    return x.template apply<T>( addr );
  }
  // const 参照
  template<class T, class... Args>
  inline T* apply_in_place( in_place_factory<Args...> const& x, void* addr ) {
    return x.template apply<T>( addr );
  }
  // 右辺値参照
  template<class T, class... Args>
  inline T* apply_in_place( in_place_factory<Args...> && x, void* addr ) {
    return x.template move_apply<T>( addr );
  }
  // const& があれば & は要らないと思うかもしれないが、
  // apply_in_place.hpp によって定義された apply_in_place が perfect forward なので、
  // & も定義しないと、一般版の apply_in_place が呼ばれてしまう（今回は特に困らないが）。
  
  
  // function template in_place
  
  // 一時オブジェクトを rvalue-reference として束縛
  // auto を使って束縛されると危険だが、 std::unique_ptr 等に重宝する
  template<class... Args>
  inline in_place_factory<Args&&...> in_place( Args&& ...args ) {
    return in_place_factory<Args&&...>( std::forward<Args&&>(args)... );
  }
  // 変数や戻り値として使える安全版
  // デフォルトでは全て値で束縛し、
  // 参照を束縛したい場合は std::ref を使う
  template<class... Args>
  inline in_place_factory<
      typename decay_and_strip<Args>::type...
  >
  bind_in_place( Args&&... args ) {
    return in_place_factory<typename decay_and_strip<Args>::type...>(
      std::forward<Args>(args)...
    );
  }
  
  // 気の利いた型変換をしない版も用意
  
  // rvalue は値として、 lvalue は参照としてキャプチャする
  // 使い回す場合は、参照なので不意な変更に注意！
  template<class... Args>
  inline in_place_factory<Args...> in_place_by_ref( Args&&...args ) {
    return in_place_factory<Args...>( std::forward<Args>(args)... );
  }
  // すべて値で束縛する
  template<class... Args>
  inline in_place_factory<
    typename std::decay<Args>::type...
  >
  in_place_by_val( Args&&... args )
  {
    return in_place_factory<typename std::decay<Args>::type...>(
      std::forward<Args>(args)...
    );
  }
  
  // タプルを in_place_factory に変換する版。
  // とりあえず詰め込んだ値を使ってオブジェクトを構築したい場合に。
  
  // 実装
  template<class Tuple, std::size_t... Indices>
  inline in_place_factory<
    typename etude::tuple_element<
      Indices, typename std::decay<Tuple>::type
    >::type...
  >
    in_place_from_tuple_( Tuple && t, etude::indices<Indices...> )
  {
    typedef in_place_factory<
      typename etude::tuple_element<
        Indices, typename std::decay<Tuple>::type
      >::type...
    > result_type;
    
    (void)t;  // サイズ 0 のタプルに対する警告避け
    return result_type( etude::tuple_forward<Indices, Tuple>(t)... );
  }
  
  // 本体
  template<class Tuple,
    class Indices = typename etude::tuple_indices<Tuple>::type
  >
  inline auto in_place_from_tuple( Tuple && t )
    -> decltype( in_place_from_tuple_( std::forward<Tuple>(t), Indices() ) )
  {
    return in_place_from_tuple_( std::forward<Tuple>(t), Indices() );
  }

}

#endif  // #ifndef ETUDE_MEMORY_INCLUDED_IN_PLACE_FACTORY_HPP_
