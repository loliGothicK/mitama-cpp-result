#pragma once
//! @cond
namespace mitama {
template <class T, class E, class /* for detection idiom */ = decltype(nullptr)>
class Result;

template <class>
class Ok;

template <class>
class Err;
}
//! @endcond