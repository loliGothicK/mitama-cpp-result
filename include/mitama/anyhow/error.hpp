#ifndef MITAMA_ANYHOW_ERROR_HPP
#define MITAMA_ANYHOW_ERROR_HPP

#include <exception>
#include <string>
#include <sstream>
#include <memory>

namespace mitama::anyhow {

  struct error {
    virtual std::string what() const = 0;
  };

  class errors: public error {
    std::shared_ptr<error>  src_;
    std::shared_ptr<error> ctx_;
  public:
    errors() = default;
    errors(const errors &) = default;
    errors &operator=(const errors &) = default;
    errors(errors &&) = default;
    errors &operator=(errors &&) = default;
    virtual ~errors() = default;

    errors(std::shared_ptr<error> src, std::shared_ptr<error> ctx)
      : src_(std::move(src)), ctx_(std::move(ctx)) {}

    std::string what() const override {
      std::stringstream ss;
      ss << "cause: " << ctx_->what() << '\n'
         << "source: " << src_->what();
      return ss.str();
    }
  };

  template<class E>
  class cause : public error {
    E err;
  public:
    cause() = default;
    cause(const cause &) = default;
    cause &operator=(const cause &) = default;

    virtual ~cause() = default;

    explicit cause(E err) noexcept : err(err) {}

    std::string what() const override {
      std::stringstream ss;
      ss << err;
      return ss.str();
    }
  };

  template<class E> cause(E) -> cause<std::decay_t<E>>;

  template<class E>
  auto anyhow(E &&err) -> std::shared_ptr<mitama::anyhow::error> {
    return std::make_shared<mitama::anyhow::cause<std::decay_t<E>>>(std::forward<E>(err));
  }

  std::ostream& operator<<(std::ostream& os, std::shared_ptr<::mitama::anyhow::error> const & err) {
    return os << err->what();
  }
}

#endif
