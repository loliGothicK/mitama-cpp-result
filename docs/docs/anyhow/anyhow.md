# anyhow 101

## Basic Concepts
**anyhow** is a library that can reduce the cost of writing error handling by treating error types as `shared_ptr` of a common base type.

```cpp
namespace mitama::anyhow {
  // common base type for errors
  struct error {
    virtual std::string what() const = 0;
  };
  // an alias template for anyhow
  template <class T>
  using result = result<void_to_monostate_t<T>, std::shared_ptr<::mitama::anyhow::error>>;
}
```

Examples:

In the first example, we replace `mitama::result` with `anyhow::result`.

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <mitama/anyhow/anyhow.hpp>

namespace anyhow = mitama::anyhow;

int main() {
  anyhow::result<int> res = mitama::ok(1);
}
```

In the second example, we make an error with `anyhow::anyhow`.
`anyhow::anyhow` creates `std::shared_ptr<anyhow::cause<T>>` from a given argument type of `T`.
An expression `mitama::err(anyhow::anyhow("error "s))` can be converted to `anyhow::result` since `anyhow::cause` inherits from `anyhow::error`.

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <mitama/anyhow/anyhow.hpp>

namespace anyhow = mitama::anyhow;
using namespace std::literals::string_literals;

int main() {
  anyhow::result<int> res = mitama::err(anyhow::anyhow("error"s));
}
```

## Error Chaining

By chaining errors, you can track where and how the error occurred, with a reason.
Use the `anyhow::errors` class for error chaining.
`anyhow::errors` has two parts: the error itself and its context.

```cpp
namespace mitama::anyhow {
  class errors: public error {
    std::shared_ptr<error> src_;
    std::shared_ptr<error> ctx_;
  public:
    // constructors

    std::string what() const override {
      std::stringstream ss;
      ss << "cause: " << ctx_->what() << ' '
         << "source: " << src_->what();
      return ss.str();
    }
  };

}
```

You can chaining error with `mitama::result::with_context()`.

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <mitama/result/result_io.hpp>
#include <mitama/anyhow/anyhow.hpp>

namespace anyhow = mitama::anyhow;
using namespace std::literals::string_literals;

struct database_t {}; // dummy

auto connect_to_db() -> anyhow::result<database_t> {
  return mitama::err(anyhow::anyhow("Failed to connect to the database."s));
}

auto read_db() -> anyhow::result<int> {
  auto conn = connect_to_db();
  return conn.map([] (auto const&) {
    // read the database and returns value
    return 42;
  }).with_context([] {
    return anyhow::anyhow("Failed to read the database.");
  });
}

int main() {
  auto res = read_db();
  
  std::cout << res << std::endl;
  // outputs:
  // err(cause: Failed to read the database. source: Failed to connect to the database.)
}
```
