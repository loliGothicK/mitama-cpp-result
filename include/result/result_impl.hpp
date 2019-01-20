#ifndef MITAMA_RESULT_IMPL
#define MITAMA_RESULT_IMPL

#include "detail.hpp"
#include "../traits/impl_traits.hpp"

namespace mitama::result
{

template <class, class = std::nullptr_t> 
class printer_friend_injector {};

template <class T>
class printer_friend_injector<Ok<T>,
                               trait::where<
                                   trait::formattable<T>>>
{
  std::ostream& print(std::ostream &os) const
  {
    return os << "Ok(" << static_cast<const Ok<T> *>(this)->x << ")";
  }
public:
  friend std::ostream &operator<<(std::ostream& os, const Ok<T> &ok) {
    return ok.print(os);
  }
};

template <class T>
class printer_friend_injector<Ok<T>,
                              trait::where<
                                  trait::formattable_range<T>>>
{
  std::ostream &print(std::ostream &os) const
  {
    auto iter = std::begin(static_cast<const Ok<T> *>(this)->x);
    os << "Ok([" << *iter;
    ++iter;
    for (; iter != std::end(static_cast<const Ok<T> *>(this)->x); ++iter)
      os << "," << *iter;
    return os << "])";
  }

public:
  friend std::ostream &operator<<(std::ostream &os, const Ok<T> &ok)
  {
    return ok.print(os);
  }
};

template <class T>
class printer_friend_injector<Err<T>,
                               trait::where<
                                   trait::formattable<T>>>
{
  std::ostream& print(std::ostream &os) const
  {
    return os << "Err(" << static_cast<const Err<T> *>(this)->x << ")";
  }
public:
  friend std::ostream &operator<<(std::ostream &os, const Err<T> &err)
  {
    return err.print(os);
  }
};

template <class T>
class printer_friend_injector<Err<T>,
                              trait::where<
                                  trait::formattable_range<T>>>
{
  std::ostream &print(std::ostream &os) const
  {
    auto iter = std::begin(static_cast<const Err<T> *>(this)->x);
    os << "Err([" << *iter;
    ++iter;
    for (; iter != std::end(static_cast<const Err<T> *>(this)->x); ++iter)
      os << "," << *iter;
    return os << "])";
  }

public:
  friend std::ostream &operator<<(std::ostream &os, const Err<T> &err)
  {
    return err.print(os);
  }
};

template <class T, class E>
class printer_friend_injector<Result<T, E>,
                              trait::where<
                                  std::disjunction<trait::formattable<T>, trait::formattable_range<T>>,
                                  std::disjunction<trait::formattable<E>, trait::formattable_range<E>>>>
{
  std::ostream &print(std::ostream &os) const
  {
    if (static_cast<Result<T, E> const *>(this)->is_ok()){
      if constexpr (trait::formattable_range<T>::value)
      {
        auto iter = std::begin(std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x);
        os << "Ok([" << *iter;
        ++iter;
        for (; iter != std::end(std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x); ++iter)
          os << "," << *iter;
        return os << "])";
      }
      else {
        return os << "Ok(" << std::get<Ok<T>>(static_cast<Result<T, E> const *>(this)->storage_).x << ")";
      }
    }
    else
    {
      if constexpr (trait::formattable_range<E>::value)
      {
        auto iter = std::begin(std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x);
        os << "Err([" << *iter;
        ++iter;
        for (; iter != std::end(std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x); ++iter)
          os << "," << *iter;
        return os << "])";
      }
      else
      {
        return os << "Err(" << std::get<Err<E>>(static_cast<Result<T, E> const *>(this)->storage_).x << ")";
      }
    }
  }
public:
  friend std::ostream &operator<<(std::ostream &os, const Result<T, E> &res)
  {
    return res.print(os);
  }
};


} // namespace rust_std::result
#endif