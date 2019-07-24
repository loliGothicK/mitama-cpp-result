//
//  compressed_pair:
//    boost::compressed_pair の C++0x 拡張
// 
//    boost::compressed_pair を rvalue-reference と piecewise-construction に対応。
//    また、 ADL ガードも搭載しています。
//    主にライブラリ実装用として使われることを想定しています。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_COMPRESSED_PAIR_HPP_
#define ETUDE_UTILITY_INCLUDED_COMPRESSED_PAIR_HPP_

#include "value_holder.hpp"
#include "piecewise_construct.hpp"
#include "emplace_construct.hpp"
#include "../types/is_constructible.hpp"
#include "../types/is_default_constructible.hpp"

#include <utility>
#include <type_traits>

namespace etude {
 namespace compressed_pair_ { // ADL 回避
 
  template<class T1, class T2>
  class compressed_pair;
  
  // 実装で毎回 ctor 書くの面倒なのでプリプロセッサ使う。
  #define ETUDE_COMPRESSED_PAIR_IMPL_CTOR_GEN_                            \
    compressed_pair_() {}                                                 \
                                                                          \
    compressed_pair_( compressed_pair_ const& ) = default;                \
    compressed_pair_( compressed_pair_ && )     = default;                \
                                                                          \
    template<class U1, class U2>                                          \
    compressed_pair_( U1 && x1, U2 && x2 )                                \
      :  first_( emplace_construct, std::forward<U1>(x1) ),               \
        second_( emplace_construct, std::forward<U2>(x2) ) {}             \
                                                                          \
    template<class Tuple1, class Tuple2>                                  \
    compressed_pair_( piecewise_construct_t, Tuple1 && x1, Tuple2 && x2 ) \
      :  first_( unpack_construct, std::forward<Tuple1>(x1) ),            \
        second_( unpack_construct, std::forward<Tuple2>(x2) ) {}          \
                                                                          \
    /* ETUDE_COMPRESSED_PAIR_IMPL_CTOR_GEN_ */
  
    // 普通の場合
    template<class T1, class T2, class = void>
    class compressed_pair_
    {
      typedef etude::value_holder<T1>  first_type;
      typedef etude::value_holder<T2> second_type;
      first_type first_;
      second_type second_;
      
     public:
      // コンストラクタ生成
      ETUDE_COMPRESSED_PAIR_IMPL_CTOR_GEN_
      
      // element accesss
      first_type&       first()       { return first_; }
      first_type const& first() const { return first_; }
      second_type&       second()       { return second_; }
      second_type const& second() const { return second_; }
      
    };
    
    // T1 が空の場合
    template<class T1, class T2>
    class compressed_pair_<T1, T2,
      typename std::enable_if<
        std::is_empty<T1>::value && !std::is_empty<T2>::value
      >::type
    >
      : private etude::value_holder<T1>
    {
      typedef etude::value_holder<T1>  first_type;
      typedef etude::value_holder<T2> second_type;
      
      typedef first_type first_;
      second_type second_;
      
     public:
      ETUDE_COMPRESSED_PAIR_IMPL_CTOR_GEN_
      
      // element accesss
      first_type&       first()       { return *this; }
      first_type const& first() const { return *this; }
      second_type&       second()       { return second_; }
      second_type const& second() const { return second_; }
      
    };
    
    // T2 が空の場合
    template<class T1, class T2>
    class compressed_pair_<T1, T2,
      typename std::enable_if<
        !std::is_empty<T1>::value && std::is_empty<T2>::value
      >::type
    >
      : private etude::value_holder<T2>
    {
      typedef etude::value_holder<T1>  first_type;
      typedef etude::value_holder<T2> second_type;
      
      first_type first_;
      typedef second_type second_;
      
     public:
      // ここは first_ と second_ の順が入れ替わるので警告避けのため手で定義
      compressed_pair_() {}
      
      compressed_pair_( compressed_pair_ const& ) = default;
      compressed_pair_( compressed_pair_ && )     = default;
      
      template<class U1, class U2>
      compressed_pair_( U1 && x1, U2 && x2 )
        : second_( emplace_construct, std::forward<U2>(x2) ),
           first_( emplace_construct, std::forward<U1>(x1) ) {}
      
      template<class Tuple1, class Tuple2>
      compressed_pair_( piecewise_construct_t, Tuple1 && x1, Tuple2 && x2 )
        : second_( unpack_construct, std::forward<Tuple2>(x2) ),
           first_( unpack_construct, std::forward<Tuple1>(x1) ) {}
      
      
      // element accesss
      first_type&       first()       { return first_; }
      first_type const& first() const { return first_; }
      second_type&       second()       { return *this; }
      second_type const& second() const { return *this; }
      
    };
    
    // 両方空の場合
    // T1, T2 が同じ型だと困るのでタグを打つ
    struct  first_tag_ {};
    struct second_tag_ {};
    
    template<class T1, class T2>
    class compressed_pair_<T1, T2,
      typename std::enable_if<
        std::is_empty<T1>::value && std::is_empty<T2>::value
      >::type
    >
      : private etude::value_holder<T1,  first_tag_>,
        private etude::value_holder<T2, second_tag_>
    {
      typedef etude::value_holder<T1,  first_tag_>  first_type;
      typedef etude::value_holder<T2, second_tag_> second_type;
      
      typedef  first_type  first_;
      typedef second_type second_;
      
     public:
      ETUDE_COMPRESSED_PAIR_IMPL_CTOR_GEN_
      
      // element accesss
      first_type&       first()       { return *this; }
      first_type const& first() const { return *this; }
      second_type&       second()       { return *this; }
      second_type const& second() const { return *this; }
      
    };
  
  #undef ETUDE_COMPRESSED_PAIR_IMPL_CTOR_GEN_
  
  // 本体
  template<class T1, class T2>
  class compressed_pair
    : private compressed_pair_<T1, T2>
  {
    typedef compressed_pair_<T1, T2> base;
    
   public:
    typedef T1  first_type;
    typedef T2 second_type;
    
    // デフォルト ctor は gcc-4.6.0 だと = default; でエラーになる…。
    template< class T1_ = T1, class T2_ = T2,
      class = typename std::enable_if<
        etude::is_default_constructible<T1_>::value &&
        etude::is_default_constructible<T2_>::value
      >::type
    >
    compressed_pair() {}
    
    // implicit move がないので（ｒｙ
    compressed_pair( compressed_pair const& ) = default;
    compressed_pair( compressed_pair && )     = default;
    
    // 未初期化状態で構築
    compressed_pair( uninitialized_t )
      : base( uninitialized, uninitialized ) {}
    
    // 二つの値の組から構築
    compressed_pair( T1 && x1, T2 && x2 )
      : base( std::forward<T1>(x1), std::forward<T2>(x2) ) {}
    // 最初の引数を型変換、 T1 const& も含む
    template< class U1,
      class = typename std::enable_if<
        std::is_convertible< U1, value_holder<T1> >::value
      >::type
    >
    compressed_pair( U1 && x1, T2 && x2 )
      : base( std::forward<U1>(x1), std::forward<T2>(x2) ) {}
    // 二番目の引数を型変換、 T2 const& も含む
    template< class U2,
      class = typename std::enable_if<
        std::is_convertible< U2, value_holder<T2> >::value
      >::type
    >
    compressed_pair( T1 && x1, U2 && x2 )
      : base( std::forward<T1>(x1), std::forward<U2>(x2) ) {}
    // より一般的なコンストラクタ、 T1 const& / T2 const& も含む
    template<class U1, class U2,
      class = typename std::enable_if<
        std::is_convertible< U1, value_holder<T1> >::value &&
        std::is_convertible< U2, value_holder<T2> >::value
      >::type
    >
    compressed_pair( U1 && x1, U2 && x2 )
      : base( std::forward<U1>(x1), std::forward<U2>(x2) ) {}
    
    // compressed_pair 間の型変換コンストラクタ
    template<class U1, class U2,
      class = typename std::enable_if<
        std::is_convertible<U1, T1>::value && std::is_convertible<U2, T2>::value
      >::type
    >
    compressed_pair( compressed_pair<U1, U2> const& src )
      : base( src.first(), src.second() ) {}
    
    template<class U1, class U2,
      class = typename std::enable_if<
        std::is_convertible<U1, T1>::value && std::is_convertible<U2, T2>::value
      >::type
    >
    compressed_pair( compressed_pair<U1, U2> && src )
      : base( std::forward<U1>( src.first() ), std::forward<U2>( src.second() ) ) {}
    
    // piecewise construction
    template<class Tuple1, class Tuple2,
      class = typename std::enable_if<
        etude::is_constructible< value_holder<T1>, unpack_construct_t, Tuple1 >::value &&
        etude::is_constructible< value_holder<T2>, unpack_construct_t, Tuple2 >::value
      >::type
    >
    compressed_pair( piecewise_construct_t, Tuple1 && t1, Tuple2 && t2 )
      : base( piecewise_construct,
          std::forward<Tuple1>(t1), std::forward<Tuple2>(t2) ) {}
    // piecewise construction で tuple の要素が 1 の場合
    template<class U1, class U2,
      class = typename std::enable_if<
        etude::is_constructible< value_holder<T1>, emplace_construct_t, U1 >::value &&
        etude::is_constructible< value_holder<T2>, emplace_construct_t, U2 >::value
      >::type
    >
    compressed_pair( emplace_construct_t, U1 && x1, U2 && x2 )
      : base( std::forward<U1>(x1), std::forward<U2>(x2) ) {}
    
    // first/second
    T1 &      first()       { return *base::first(); }
    T1 const& first() const { return *base::first(); }
    T2 &      second()       { return *base::second(); }
    T2 const& second() const { return *base::second(); }
    
    typename std::remove_const<T1>::type&& move_first() {
      return *std::move( base::first() );
    }
    typename std::remove_const<T2>::type&& move_second() {
      return *std::move( base::second() );
    }
    
    // swap
    void swap( compressed_pair& other ) {
      base::first().swap( other.base::first() );
      base::second().swap( other.base::second() );
    }
    
  };
  
  // non-member functions
  
  template<class T1, class T2>
  inline void swap( compressed_pair<T1, T2>& one, compressed_pair<T1, T2>& another ) {
    one.swap( another );
  }
  
  template<class T1, class T2>
  inline T1& get_first( compressed_pair<T1, T2>& x ) {
    return x.first();
  }
  template<class T1, class T2>
  inline T1 const& get_first( compressed_pair<T1, T2> const& x ) {
    return x.first();
  }
  template<class T1, class T2>
  inline typename std::remove_const<T1>::type&&
  get_first( compressed_pair<T1, T2> && x ) {
    return x.move_first();
  }
  
  template<class T1, class T2>
  inline T2& get_second( compressed_pair<T1, T2>& x ) {
    return x.second();
  }
  template<class T1, class T2>
  inline T2 const& get_second( compressed_pair<T1, T2> const& x ) {
    return x.second();
  }
  template<class T1, class T2>
  inline typename std::remove_const<T2>::type&&
  get_second( compressed_pair<T1, T2> && x ) {
    return x.move_second();
  }
 
 }  // namespace compressed_pair_;
 using namespace compressed_pair_;
}

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_COMPRESSED_PAIR_HPP_
