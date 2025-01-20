#pragma once

#include <mitama/mitamagic/format.hpp>
#include <mitama/result/factory/err.hpp>

#include <fmt/core.h>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace mitama::anyhow
{

struct error
{
  virtual ~error() = default;
  virtual std::string what() const = 0;
  virtual std::shared_ptr<error> context(std::shared_ptr<error>) = 0;
};

class errors final : public error, public std::enable_shared_from_this<errors>
{
  std::vector<std::shared_ptr<error>> errs_;

public:
  errors() = default;
  errors(const errors&) = default;
  errors& operator=(const errors&) = default;
  errors(errors&&) = default;
  errors& operator=(errors&&) = default;

  ~errors() override = default;
  template <class... Errors>
  errors(Errors&&... errs) : errs_{ std::forward<Errors>(errs)... }
  {
  }

  std::shared_ptr<error> context(std::shared_ptr<error> ctx) override
  {
    errs_.emplace_back(std::move(ctx));
    return shared_from_this();
  }

  auto chain() const
  {
    return std::vector{ errs_.crbegin(), errs_.crend() };
  }

  auto root_cause() const -> std::shared_ptr<mitama::anyhow::error>
  {
    if (errs_.empty())
    {
      return nullptr;
    }
    else
    {
      return errs_.back();
    }
  }

  // 1.
  // ```
  // Failed to read the database.
  // ```
  //
  // 2.
  // ```
  // Failed to read the database.
  //
  // Caused by:
  //     Failed to connect to the database.
  // ```
  //
  // 3.
  // ```
  // Failed to read the database.
  //
  // Caused by:
  //     0: Failed to write the database.
  //     1: Failed to connect to the database.
  // ```
  std::string what() const override
  {
    std::stringstream ss;
    if (errs_.empty())
    {
      return ss.str();
    }

    if (errs_.size() == 1)
    {
      ss << (*errs_.crbegin())->what() << "\n";
    }
    else
    {
      auto iter = errs_.crbegin();
      ss << (*iter)->what() << "\n\n"
         << "Caused by:\n";
      ++iter;

      if (errs_.size() == 2)
      {
        ss << "    " << (*iter)->what() << "\n";
      }
      else
      {
        for (int i = 0; iter != errs_.crend(); ++iter, ++i)
        {
          ss << "    " << i << ": " << (*iter)->what() << "\n";
        }
      }
    }
    return ss.str();
  }
};

template <class E>
class cause final : public error, public std::enable_shared_from_this<cause<E>>
{
  E err;

public:
  cause() = default;
  cause(const cause&) = default;
  cause& operator=(const cause&) = default;

  ~cause() override = default;

  explicit cause(E err) noexcept : err(err) {}

  std::shared_ptr<error> context(std::shared_ptr<error> ctx) override
  {
    return std::make_shared<errors>(
        std::enable_shared_from_this<cause<E>>::shared_from_this(),
        std::move(ctx)
    );
  }

  std::string what() const override
  {
    std::stringstream ss;
    ss << err;
    return ss.str();
  }
};

template <class E>
cause(E) -> cause<std::decay_t<E>>;

template <class E>
inline auto
anyhow(E&& err) -> std::shared_ptr<mitama::anyhow::error>
{
  return std::make_shared<mitama::anyhow::cause<std::decay_t<E>>>(
      std::forward<E>(err)
  );
}
template <class... E>
  requires(fmt::is_formattable<E>::value && ...)
inline auto
anyhow(fmt::format_string<E...> f, E&&... args)
{
  return anyhow(fmt::format(f, std::forward<E>(args)...));
}

inline std::ostream&
operator<<(
    std::ostream& os, const std::shared_ptr<::mitama::anyhow::error>& err
)
{
  return os << err->what();
}

template <class Err, class... Args>
inline auto
err(Args&&... args) -> mitama::err_t<std::shared_ptr<Err>>
  requires std::is_base_of_v<mitama::anyhow::error, Err>
{
  return mitama::err(std::make_shared<Err>(std::forward<Args>(args)...));
}

} // namespace mitama::anyhow

template <>
struct fmt::formatter<std::shared_ptr<mitama::anyhow::error>>
    : ostream_formatter
{
};

#define MITAMA_BAIL(...) \
  return ::mitama::err(::mitama::anyhow::anyhow(__VA_ARGS__))

#define MITAMA_ENSURE(COND, ...) \
  do                             \
  {                              \
    if (!(COND))                 \
    {                            \
      MITAMA_BAIL(__VA_ARGS__);  \
    }                            \
  } while (false)
