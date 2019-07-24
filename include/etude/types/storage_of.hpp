//
//  storage_of :
//    与えられた型全てを格納できるストレージクラス
//    
//    storage_of<Ts...>::type は、 Ts... で指定される全ての型を
//    格納することのできる POD 型に定義されます。
//    
//    これは
//    std::aligned_storage<storage_of<Ts...>::size, storage_of<Ts...>::align>::type
//    と同じ型になります。
//    storage_of<Ts...>::size および storage_of<Ts...>::align は、それぞれ
//    storage_size<Ts>::value... および storage_align<Ts>::value... のうち最大の値です。
//    
//  Copyright (C) 2010-11  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_STORAGE_OF_HPP_
#define ETUDE_TYPES_INCLUDED_STORAGE_OF_HPP_

#include <type_traits>
#include "storage_size.hpp"
#include "storage_align.hpp"
#include "maximum_of.hpp"
#include "all_of_c.hpp"

namespace etude {

  // 与えられた型全てを格納できるストレージクラス
  template<class... Ts>
  struct storage_of
  {
    static_assert( all_of_c<storage_size<Ts>::is_defined...>::value,
      "each type of template parameter Ts... must be complete." );
    
    static const std::size_t size =
      maximum_of<std::size_t, 1, storage_size<Ts>::value...>::value;
    static const std::size_t align =
      maximum_of<std::size_t, 1, storage_align<Ts>::value...>::value;
    
    static const bool is_empty =
      all_of_c<std::is_empty<Ts>::value...>::value;
    
    typedef typename std::aligned_storage<size, align>::type type;
    
  };

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_STORAGE_OF_HPP_
