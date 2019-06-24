#ifndef MITAMA_RESULT_IMPL_HPP
#define MITAMA_RESULT_IMPL_HPP

#include <result/detail/meta.hpp>
#include <result/traits/impl_traits.hpp>
#include <result/traits/Deref.hpp>
#include <result/detail/dangling.hpp>
#include <optional>
#include <functional>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/utility/in_place_factory.hpp>

namespace mitama {

template <class, class = void>
class unwrap_or_default_friend_injector
{
public:
  void unwrap_or_default() const = delete;
};

template <mutability _mu, class T, class E>
class unwrap_or_default_friend_injector<basic_result<_mu, T, E>,
                                        std::enable_if_t<std::disjunction_v<std::is_default_constructible<T>, std::is_aggregate<T>>>>
{
public:
  T unwrap_or_default() const
  {
    if constexpr (std::is_aggregate_v<T>){
      return static_cast<basic_result<_mu, T, E> const *>(this)->is_ok()
        ? static_cast<basic_result<_mu, T, E> const *>(this)->unwrap()
        : T{};
    }
    else {
      return static_cast<basic_result<_mu, T, E> const *>(this)->is_ok()
        ? static_cast<basic_result<_mu, T, E> const *>(this)->unwrap()
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

/// @brief
///   impl<_mutability, T, E> basic_result<_mutability, Option<T>, E>
template <mutability _mutability, class T, class E>
class transpose_friend_injector<basic_result<_mutability, T, E>,
                                std::enable_if_t<meta::is_optional<std::decay_t<T>>::value>>
{
  using optional_type = meta::remove_cvr_t<typename meta::repack<T>::template type<basic_result<_mutability, typename meta::remove_cvr_t<T>::value_type, E>>>;
public:
  optional_type transpose() const &
  {
    if constexpr (meta::is_boost_optional<meta::remove_cvr_t<T>>::value) {
      if (static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok()) {
        if (auto const& opt = boost::get<success<T>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x; opt) {
          return optional_type(boost::in_place(mitama::in_place_ok, opt.value()));
        }
        else {
          return boost::none;
        }
      }
      else {
          return optional_type(boost::in_place(mitama::in_place_err, boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x));
      }
    }
    else {
      if (static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok()) {
        if (auto const& opt = std::get<success<T>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x; opt) {
          return optional_type(std::in_place, mitama::in_place_ok, opt.value());
        }
        else {
          return std::nullopt;
        }
      }
      else {
          return optional_type(std::in_place, mitama::in_place_err, std::get<failure<E>>(static_cast<basic_result<_mutability, T, E> const*>(this)->storage_).x);
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

/// @brief
/// impl<_mutability, T, E> basic_result<_mutability, T, E>
/// where
///   T: Deref
///   E: Deref
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
  using const_deref_ok_result = basic_result<_mutability, meta::remove_cvr_t<typename traits::Deref<T>::Target> const&, meta::remove_cvr_t<E> const&>;
  using const_deref_err_result = basic_result<_mutability, meta::remove_cvr_t<T> const&, meta::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using const_deref_result = basic_result<_mutability, meta::remove_cvr_t<typename traits::Deref<T>::Target> const&, meta::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using dangling_deref_ok_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<T>::Target>>>, dangling<std::reference_wrapper<std::remove_reference_t<E>>>>;
  using dangling_deref_err_result = basic_result<_mutability, dangling<std::remove_reference_t<T>&>, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<E>::Target>>>>;
  using dangling_deref_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<T>::Target>>>, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<E>::Target>>>>;
public:

  constexpr auto deref_ok() & -> deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_ok_result{in_place_ok, *boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_ok_result{in_place_err, boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref_err() & -> deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_err_result{in_place_ok, boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_err_result{in_place_err, *boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref() & -> deref_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_result{in_place_ok, *boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_result{in_place_err, *boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }



  constexpr auto deref_ok() const & -> const_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_ok_result{in_place_ok, *boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_ok_result{in_place_err, boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref_err() const & -> const_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_err_result{in_place_ok, boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_err_result{in_place_err, *boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref() const & -> const_deref_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_result{in_place_ok, *boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_result{in_place_err, *boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }



  constexpr auto deref_ok() && -> dangling_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_ok_result{in_place_ok, std::ref(*boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x)};
    }
    else {
      return dangling_deref_ok_result{in_place_err, boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr auto deref_err() && -> dangling_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_err_result{in_place_ok, boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return dangling_deref_err_result{in_place_err, std::ref(*boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x)};
    }
  }
  constexpr auto deref() && -> dangling_deref_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_result{in_place_ok, std::ref(*boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x)};
    }
    else {
      return dangling_deref_result{in_place_err, std::ref(*boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x)};
    }
  }

  constexpr void deref_ok() const && = delete;
  constexpr void deref_err() const && = delete;
  constexpr void deref() const && = delete;

};

/// @brief
/// impl<_mutability, T, E> basic_result<_mutability, T, E>
/// where
///   T: Deref
template <mutability _mutability, class T, class E>
class deref_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                traits::is_dereferencable<T>,
                                std::negation<traits::is_dereferencable<E>>
                            >>>
{
  using deref_ok_result = basic_result<_mutability, std::remove_reference_t<typename traits::Deref<T>::Target>&, std::remove_reference_t<E>&>;
  using const_deref_ok_result = basic_result<_mutability, meta::remove_cvr_t<typename traits::Deref<T>::Target> const&, meta::remove_cvr_t<E> const&>;
  using dangling_deref_ok_result = basic_result<_mutability, dangling<std::reference_wrapper<std::remove_reference_t<typename traits::Deref<T>::Target>>>, dangling<std::reference_wrapper<std::remove_reference_t<E>>>>;
public:
  constexpr auto deref_ok() & -> deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_ok_result{in_place_ok, *boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_ok_result{in_place_err, boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref_err() & = delete;
  constexpr void deref() & = delete;



  constexpr auto deref_ok() const & -> const_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_ok_result{in_place_ok, *boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_ok_result{in_place_err, boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref_err() const & = delete;
  constexpr void deref() const & = delete;



  constexpr auto deref_ok() && -> dangling_deref_ok_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_ok_result{in_place_ok, std::ref(*boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x)};
    }
    else {
      return dangling_deref_ok_result{in_place_err, boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref_err() && = delete;
  constexpr void deref() && = delete;

  constexpr void deref_ok() const && = delete;
  constexpr void deref_err() const && = delete;
  constexpr void deref() const && = delete;

};

/// @brief
/// impl<_mutability, T, E> basic_result<_mutability, T, E>
/// where
///   E: Deref
template <mutability _mutability, class T, class E>
class deref_friend_injector<basic_result<_mutability, T, E>, 
                            std::enable_if_t<
                              std::conjunction_v<
                                std::negation<traits::is_dereferencable<T>>,
                                traits::is_dereferencable<E>
                            >>>
{
  using deref_err_result = basic_result<_mutability, std::remove_reference_t<T>&, std::remove_reference_t<typename traits::Deref<E>::Target>&>;
  using const_deref_err_result = basic_result<_mutability, meta::remove_cvr_t<T> const&, meta::remove_cvr_t<typename traits::Deref<E>::Target> const&>;
  using dangling_deref_err_result = basic_result<_mutability, dangling<std::remove_reference_t<T>&>, dangling<std::remove_reference_t<typename traits::Deref<E>::Target>&>>;
public:
  constexpr void deref_ok() & = delete;
  constexpr auto deref_err() & -> deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return deref_err_result{in_place_ok, boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return deref_err_result{in_place_err, *boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref() & = delete;



  constexpr void deref_ok() const & = delete;
  constexpr auto deref_err() const & -> const_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return const_deref_err_result{in_place_ok, boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return const_deref_err_result{in_place_err, *boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
  }
  constexpr void deref() const & = delete;



  constexpr void deref_ok() && = delete;
  constexpr auto deref_err() && -> dangling_deref_err_result {
    if ( static_cast<basic_result<_mutability, T, E> const *>(this)->is_ok() ) {
      return dangling_deref_err_result{in_place_ok, boost::get<success<T>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x};
    }
    else {
      return dangling_deref_err_result{in_place_err, std::ref(*boost::get<failure<E>>(static_cast<basic_result<_mutability, T, E>*>(this)->storage_).x)};
    }
  }
  constexpr void deref() && = delete;

  constexpr void deref_ok() const && = delete;
  constexpr void deref_err() const && = delete;
  constexpr void deref() const && = delete;

};


} // namespace rust_std::result
#endif
