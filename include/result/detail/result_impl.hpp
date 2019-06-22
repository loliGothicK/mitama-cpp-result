#ifndef MITAMA_RESULT_IMPL
#define MITAMA_RESULT_IMPL

#include "detail.hpp"
#include <result/traits/impl_traits.hpp>
#include <result/traits/Deref.hpp>
#include <result/detail/dangling.hpp>
#include <optional>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/utility/in_place_factory.hpp>

namespace mitama::detail {
template < class >
struct is_std_optional: std::false_type {};
template < class >
struct is_boost_optional: std::false_type {};
template < class T >
struct is_std_optional<std::optional<T>>: std::true_type {};
template < class T >
struct is_boost_optional<boost::optional<T>>: std::true_type {};

template < class > struct repack;

template < template < class > class Opt, class T >
struct repack<Opt<T>> {
  template < class U >
  using type = Opt<U>;
};
}

namespace mitama {

template < class T >
struct is_optional
  : std::disjunction<
      detail::is_std_optional<detail::remove_cvr_t<T>>,
      detail::is_boost_optional<detail::remove_cvr_t<T>>
    >
{};

class in_place_ok_t {};
inline constexpr in_place_ok_t in_place_ok = {};

class in_place_err_t {};
inline constexpr in_place_err_t in_place_err = {};

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

template < >
class printer_friend_injector<Ok<std::monostate>>
{
public:
  friend std::ostream &operator<<(std::ostream &os, Ok<std::monostate> const&)
  {
    return os << "Ok(())";
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

template < >
class printer_friend_injector<Err<std::monostate>>
{
public:
  friend std::ostream &operator<<(std::ostream &os, Err<std::monostate> const&)
  {
    return os << "Err(())";
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

template <mutability _mutability, class T, class E>
class printer_friend_injector<basic_result<_mutability, T, E>,
                              trait::where<
                                  std::disjunction<trait::formattable<T>, trait::formattable_range<T>>,
                                  std::disjunction<trait::formattable<E>, trait::formattable_range<E>>>>
{
  std::ostream &print(std::ostream &os) const
  {
    if (static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok()){
      if constexpr (trait::formattable_range<T>::value)
      {
        auto iter = std::begin(boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E> const *>(this)->storage_).x);
        os << "Ok([" << *iter;
        ++iter;
        for (; iter != std::end(boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E> const *>(this)->storage_).x); ++iter)
          os << "," << *iter;
        return os << "])";
      }
      else if constexpr (std::is_same_v<std::decay_t<T>, std::monostate>) {
        return os << "Ok(())";
      }
      else {
        return os << "Ok(" << boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E> const *>(this)->storage_).x << ")";
      }
    }
    else
    {
      if constexpr (trait::formattable_range<E>::value)
      {
        auto iter = std::begin(boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E> const *>(this)->storage_).x);
        os << "Err([" << *iter;
        ++iter;
        for (; iter != std::end(boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E> const *>(this)->storage_).x); ++iter)
          os << "," << *iter;
        return os << "])";
      }
      else if constexpr (std::is_same_v<std::decay_t<E>, std::monostate>) {
        return os << "Err(())";
      }
      else
      {
        return os << "Err(" << boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E> const *>(this)->storage_).x << ")";
      }
    }
  }
public:
  friend std::ostream &operator<<(std::ostream &os, const basic_result<_mutability, T, E> &res)
  {
    return res.print(os);
  }
};

template <class, class = void>
class unwrap_or_default_friend_injector
{
public:
  void unwrap_or_default() const = delete;
};

template <mutability _mutability, class T, class E>
class unwrap_or_default_friend_injector<basic_result<_mutability, T, E>,
                                        std::enable_if_t<std::disjunction_v<std::is_default_constructible<T>, std::is_aggregate<T>>>>
{
public:
  T unwrap_or_default() const
  {
    if constexpr (std::is_aggregate_v<T>){
      return static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok()
        ? static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()
        : T{};
    }
    else {
      return static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok()
        ? static_cast<basic_result<_mutability, T, E> const *>(this)->unwrap()
        : T();
    }
  }
};

template <class, class = void>
class transpose_friend_injector
{
public:
  void transpose() const = delete;
};


template <mutability _mutability, class T, class E>
class transpose_friend_injector<basic_result<_mutability, T, E>,
                                std::enable_if_t<is_optional<std::decay_t<T>>::value>>
{
  using optional_type = detail::remove_cvr_t<typename detail::repack<T>::template type<basic_result<_mutability, typename detail::remove_cvr_t<T>::value_type, E>>>;
public:
  optional_type transpose() const &
  {
    if constexpr (detail::is_boost_optional<detail::remove_cvr_t<T>>::value) {
      if (static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok()) {
        if (auto const& opt = boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x; opt) {
          return optional_type(boost::in_place(mitama::in_place_ok, opt.value()));
        }
        else {
          return boost::none;
        }
      }
      else {
          return optional_type(boost::in_place(mitama::in_place_err, boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x));
      }
    }
    else {
      if (static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok()) {
        if (auto const& opt = std::get<Ok<T>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x; opt) {
          return optional_type(std::in_place, mitama::in_place_ok, opt.value());
        }
        else {
          return std::nullopt;
        }
      }
      else {
          return optional_type(std::in_place, mitama::in_place_err, std::get<Err<E>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x);
      }
    }
  }
};


template <class, class = void>
class deref_friend_injector
{
public:
  void deref() const = delete;
  void deref_ok() const = delete;
  void deref_err() const = delete;
};


template <mutability _mutability, class T, class E>
class deref_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                traits::is_dereferencable<T>,
                                traits::is_dereferencable<E>
                            >>>
{
  using deref_ok_result = basic_result<_mutability, std::remove_reference_t<typename traits::Deref<T>::Target>&, std::remove_reference_t<E>&>;
  using deref_err_result = basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<typename traits::Deref<E>::Target>&>;
  using deref_result = basic_result<_mutability, std::remove_reference_t<typename traits::Deref<T>::Target>&, std::remove_reference_t<typename traits::Deref<E>::Target>&>;
  using const_deref_ok_result = basic_result<_mutability, detail::remove_cvr_t<typename traits::Deref<T>::Target> const&, detail::remove_cvr_t<E> const&>;
  using const_deref_err_result = basic_result<_mutability, detail::remove_cvr_t<T> const&, detail::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using const_deref_result = basic_result<_mutability, detail::remove_cvr_t<typename traits::Deref<T>::Target> const&, detail::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using dangling_deref_ok_result = basic_result<_mutability, dangling<std::remove_reference_t<typename traits::Deref<T>::Target>&>, dangling<std::remove_reference_t<E>&>>;
  using dangling_deref_err_result = basic_result<_mutability, dangling<std::remove_reference_t<T>&>, dangling<std::remove_reference_t<typename traits::Deref<E>::Target>&>>;
  using dangling_deref_result = basic_result<_mutability, dangling<std::remove_reference_t<typename traits::Deref<T>::Target>&>, dangling<std::remove_reference_t<typename traits::Deref<E>::Target>&>>;
public:
  constexpr auto deref_ok() & -> deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_ok_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_ok_result{in_place_err, boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref_err() & -> deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_err_result{in_place_ok, boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_err_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref() & -> deref_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }



  constexpr auto deref_ok() const & -> const_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_ok_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_ok_result{in_place_err, boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref_err() const & -> const_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_err_result{in_place_ok, boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_err_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref() const & -> const_deref_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }



  constexpr auto deref_ok() && -> dangling_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_ok_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return dangling_deref_ok_result{in_place_err, boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref_err() && -> dangling_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_err_result{in_place_ok, boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return dangling_deref_err_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref() && -> dangling_deref_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return dangling_deref_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }

  constexpr void deref_ok() const && = delete;
  constexpr void deref_err() const && = delete;
  constexpr void deref() const && = delete;

};

template <mutability _mutability, class T, class E>
class deref_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                traits::is_dereferencable<T>,
                                std::negation<traits::is_dereferencable<E>>
                            >>>
{
  using deref_ok_result = basic_result<_mutability, std::remove_reference_t<typename traits::Deref<T>::Target>&, std::remove_reference_t<E>&>;
  using const_deref_ok_result = basic_result<_mutability, detail::remove_cvr_t<typename traits::Deref<T>::Target> const&, detail::remove_cvr_t<E> const&>;
  using dangling_deref_ok_result = basic_result<_mutability, dangling<std::remove_reference_t<typename traits::Deref<T>::Target>&>, dangling<std::remove_reference_t<E>&>>;
public:
  constexpr auto deref_ok() & -> deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_ok_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_ok_result{in_place_err, boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref_err() & = delete;
  constexpr void deref() & = delete;



  constexpr auto deref_ok() const & -> const_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_ok_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_ok_result{in_place_err, boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref_err() const & = delete;
  constexpr void deref() const & = delete;



  constexpr auto deref_ok() && -> dangling_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_ok_result{in_place_ok, *boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return dangling_deref_ok_result{in_place_err, boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref_err() && = delete;
  constexpr void deref() && = delete;

  constexpr void deref_ok() const && = delete;
  constexpr void deref_err() const && = delete;
  constexpr void deref() const && = delete;

};

template <mutability _mutability, class T, class E>
class deref_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                std::negation<traits::is_dereferencable<T>>,
                                traits::is_dereferencable<E>
                            >>>
{
  using deref_err_result = basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<typename traits::Deref<E>::Target>&>;
  using const_deref_err_result = basic_result<_mutability, detail::remove_cvr_t<T> const&, detail::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using dangling_deref_err_result = basic_result<_mutability, dangling<std::remove_reference_t<T>&>, dangling<std::remove_reference_t<typename traits::Deref<E>::Target>&>>;
public:
  constexpr void deref_ok() & = delete;
  constexpr auto deref_err() & -> deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_err_result{in_place_ok, boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_err_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref() & = delete;



  constexpr void deref_ok() const & = delete;
  constexpr auto deref_err() const & -> const_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_err_result{in_place_ok, boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_err_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref() const & = delete;



  constexpr void deref_ok() && = delete;
  constexpr auto deref_err() && -> dangling_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_err_result{in_place_ok, boost::get<Ok<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return dangling_deref_err_result{in_place_err, *boost::get<Err<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref() && = delete;

  constexpr void deref_ok() const && = delete;
  constexpr void deref_err() const && = delete;
  constexpr void deref() const && = delete;

};


} // namespace rust_std::result
#endif
