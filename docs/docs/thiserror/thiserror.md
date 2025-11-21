# thiserror 101

**thiserror** is a library that makes it easy to write domain-specific error types.
We can embed the reason for the error directly into the type.

For example, if you want to write `data_store_error`, you can write the following:

```cpp
#include <mitama/thiserror/thiserror.hpp>

struct data_store_error : mitama::thiserror::derive_error {
  using disconnect
      = error<"data store disconnected">;
  using redaction
      = error<"for key `{0}` isn't available", std::string>;
  using invalid_header
      = error<"(expected {0}, found {1})", std::string, std::string>;
  using unknown
      = error<"unknown data store error">;
};
```

The first template argument of `mitama::thiserror::error` is embedded with a format string.
The format string follows the `std::format` grammar.
The second template argument onwards is a list of formattable arguments.

`mitama::thiserror::error` inherits from `anyhow::error`, so it can be used together with anyhow.
The factory function `anyhow::failure` is used to create a `shared_ptr` for `thiserror::error`.

```cpp
anyhow::result<int> data
    = anyhow::failure<data_store_error::disconnect>();
```

Examples:

```cpp
// begin example
#include <mitama/result/result.hpp>
#include <mitama/result/result_io.hpp>
#include <mitama/anyhow/anyhow.hpp>
#include <mitama/thiserror/thiserror.hpp>

namespace anyhow = mitama::anyhow;
using namespace std::literals;

struct data_store_error : mitama::thiserror::derive_error {
  using disconnect
        = error<"data store disconnected">;
  using redaction
        = error<"for key `{0}` isn't available", std::string>;
  using invalid_header
        = error<"(expected {0}, found {1})", std::string, std::string>;
  using unknown
        = error<"unknown data store error">;
};

int main() {
  anyhow::result<int> data
      = anyhow::failure<data_store_error::redaction>("invalid key");
  auto res = data
      .with_context([] { return anyhow::anyhow("data store failed."s); });
}
```
