//
//  is_trivially_destructible :
//    std::is_trivially_destructible の別名
//
//    gcc-4.8 以前では std::has_trivial_destructor という名前だったので
//    その差分を吸収するためのヘッダです
//
//  Copyright (C) 2013  Takaya Saito (SubaruG)
//    Distributed under the Boost Software License, Version 1.0.
//    http://www.boost.org/LICENSE_1_0.txt
//
#ifndef ETUDE_TYPES_INCLUDED_IS_TRIVIALLY_XXX_HPP_
#define ETUDE_TYPES_INCLUDED_IS_TRIVIALLY_XXX_HPP_

#include <type_traits>

namespace etude {

#if defined(__GNUC__) && \
  __GNUC__ == 4 && __GNUC_MINOR__ <= 7

  template<class T>
  struct is_trivially_destructible
    : std::has_trivial_destructor<T>::type {};

#else

  using std::is_trivially_destructible;

#endif

} // namespace etude

#endif  // #ifndef ETUDE_TYPES_INCLUDED_IS_TRIVIALLY_XXX_HPP_
