# Introduction

`Mitama.Result` is a header only C++17 library for error handling.


## Prerequisites and installation

### Prerequisites

| Compiler/Toolchain |                       Status                       |
| :----------------: | :------------------------------------------------: |
|   clang >= 7.0.0   | Testing on CircleCI; tested on each push to GitHub |
|    gcc >= 8.3.0    | Testing on CircleCI; tested on each push to GitHub |
|  boost >= 1.67.0   | Testing on CircleCI; tested on each push to GitHub |

More specifically, `Mitama.Result` requires a compiler/standard library supporting the following C++17 features:

- constexpr if
- constexpr lambda
- inline variables
- fold expressions
- class template deduction and deduction guide
- All the C++17 type traits from the `<type_traits>` header
- `std::{invoke, apply}` from the `<functional>` header
- `std::string_view` from the `<string_view>` header
- `std::monostate` from the `<variant>` header

And requires a Boost supporting the following libraries:

- `boost::format` from the `<boost/format.hpp>` header
- `boost::hana::{fix, overload, overload_linearly}` from the `<boost/hana/functional/{fix, overload, overload_linearly}.hpp>` header

### Installation

#### CMake

- First, clone mitama-cpp-result repository.

```shell
$ git clone https://github.com/LoliGothick/mitama-cpp-result.git
```

- Second, run CMake and Make commands:

```shell
$ cd mitama-cpp-result
$ cmake ..
$ make install
```


#### Manually

- First, clone mitama-cpp-result repository.

```shell
$ git clone https://github.com/LoliGothick/mitama-cpp-result.git
```

- Second, add `path/to/mitama-cpp/include` to include path.


## Basic Usage

Here is a bad code.

```cpp
bool func(int a) {
  if ( first check )
    return false;
  if ( second check )
    return false;
  if ( third check )
    return false;
  // function body...
  return true;
}
// ...
assert(func(42));
```

If this program fail to assert, you don't know the reason for the error.

Rewrite this code using result.

```cpp
auto func(int a) -> mitama::result<int, std::string> {
  if ( first check )
    return mitama::failure("first check failed"); // early return
  if ( second check )
    return mitama::failure("second check failed"); // early return
  if ( third check )
    return mitama::failure("third check failed"); // early return
  // function body...
  return mitama::success(42);
}
// ...
int value = func(42).unwrap();
```

Even if this program fail to assert, you can get the reason for the error.
