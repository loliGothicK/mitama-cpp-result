# Introduction

`Mitama.Result` is a header only C++23 library for error handling.


## Installation


### Dependencies

* No external dependencies


### Supported OS / Compiler / Language Standard

- OS
    - Linux
    - Apple Darwin
    - (Windows)

- Compilers
    - gcc >= 11
    - clang >= 15
    - Latest preview version of MSVC only

- Language Standards
    - C++23
    - (MSVC: only support `/std:c++latest`)


### CMake

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


### Manually

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
