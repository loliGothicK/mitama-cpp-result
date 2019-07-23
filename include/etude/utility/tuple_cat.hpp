//
//  tuple_cat:
//    渡された Tuple を結合した std::tuple を返す
// 
//    渡されたタプルのようなものを結合して std::tuple に変換します。
//  
//  Copyright (C) 2011  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_UTILITY_INCLUDED_TUPLE_CAT_HPP_
#define ETUDE_UTILITY_INCLUDED_TUPLE_CAT_HPP_

#include "../unpacked_tuple.hpp"
#include "../unpack.hpp"
#include "../types/is_tuple.hpp"
#include "../types/tuple_element.hpp"
#include "../types/tuple_indices.hpp"
#include "../types/get_type_or.hpp"
#include "tuple_get.hpp"
#include "forward_as_tuple.hpp"

#include <utility>
#include <type_traits>

namespace etude {

  // 結果のメタ関数
  template<class... Tuples>
  struct tuple_concatenated {};
  
  // 一引数
  template<class Tuple>
  struct tuple_concatenated<Tuple> :
    std::enable_if<
      etude::is_tuple<typename std::decay<Tuple>::type>::value,
      typename std::decay<Tuple>::type
    >
  {};
  
  // 実装
  template<class T1, class I1, class T2, class I2>
  struct tuple_concatenated_ {};
  
  template<
    class Tuple1, std::size_t... Indices1,
    class Tuple2, std::size_t... Indices2
  >
  struct tuple_concatenated_<
    Tuple1, etude::indices<Indices1...>,
    Tuple2, etude::indices<Indices2...>
  >
  {
    typedef std::tuple<
      typename etude::tuple_element< Indices1,
        typename std::decay<Tuple1>::type
      >::type...,
      typename etude::tuple_element< Indices2,
        typename std::decay<Tuple2>::type
      >::type...
    > type;
  };
  
  template<class Tuple1, class Tuple2>
  struct tuple_concatenated<Tuple1, Tuple2> :
    tuple_concatenated_<
      Tuple1, typename etude::get_type_or<etude::tuple_indices<Tuple1>>::type,
      Tuple2, typename etude::get_type_or<etude::tuple_indices<Tuple2>>::type
    >
  {
  };
  
  template<class Tuple1, class Tuple2, class... Tuples>
  struct tuple_concatenated<Tuple1, Tuple2, Tuples...> :
    tuple_concatenated<
      typename etude::get_type_or<etude::tuple_concatenated<Tuple1, Tuple2>>::type,
      Tuples...
    >
  {
  };

  // 実装、兼インデックス指定版
  template<
    class Tuple1, std::size_t... Indices1,
    class Tuple2, std::size_t... Indices2
  >
  inline std::tuple<
    typename etude::tuple_element< Indices1,
      typename std::decay<Tuple1>::type
    >::type...,
    typename etude::tuple_element< Indices2,
      typename std::decay<Tuple2>::type
    >::type...
  >
  tuple_cat_(
    unpacked_tuple<Tuple1, Indices1...> && t1, unpacked_tuple<Tuple2, Indices2...> && t2
  ){
    typedef std::tuple<
      typename etude::tuple_element< Indices1,
        typename std::decay<Tuple1>::type
      >::type...,
      typename etude::tuple_element< Indices2,
        typename std::decay<Tuple2>::type
      >::type...
    > result_type;
    
    (void)t1, (void)t2;  // 警告避け
    return result_type(
      etude::move<Indices1>(t1)..., etude::move<Indices2>(t2)...
    );
  }

  // 本体
  template< class Tuple1, class Tuple2,
    class Result = typename etude::tuple_concatenated<Tuple1, Tuple2>::type
  >
  inline Result tuple_cat( Tuple1 && t1, Tuple2 && t2 ) {
    return etude::tuple_cat_(
      etude::unpack( std::forward<Tuple1>(t1) ),
      etude::unpack( std::forward<Tuple2>(t2) )
    );
  }
  
  // 一引数
  template< class Tuple,
    class Result = typename etude::tuple_concatenated<Tuple>::type
  >
  inline Result tuple_cat( Tuple && t ) {
    return std::forward<Tuple>(t);
  }
  
  // 三引数以上
  template< class Tuple1, class Tuple2, class... Tuples,
    class Result = typename etude::tuple_concatenated<Tuple1, Tuple2, Tuples...>::type,
    class = typename std::enable_if<( sizeof...(Tuples) > 0 )>::type
  >
  inline Result tuple_cat( Tuple1 && t1, Tuple2 && t2, Tuples&&... ts ) {
    return etude::tuple_cat( 
      etude::tuple_cat( std::forward<Tuple1>(t1), std::forward<Tuple2>(t2) ),
      std::forward<Tuples>(ts)...
    );
  }

} // namespace etude

#endif  // #ifndef ETUDE_UTILITY_INCLUDED_TUPLE_CAT_HPP_
