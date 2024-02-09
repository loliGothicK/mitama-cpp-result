# mitama-cpp-result

**mitama-cpp-result** is the C++20 libraries for error handling without exceptions.

**mitama-cpp-result** provides `result<T, E>`, `maybe<T>`, and associated monadic functions
(like [Result](https://doc.rust-lang.org/std/result/enum.Result.html)
and [Option](https://doc.rust-lang.org/std/option/enum.Option.html) in Programming Language Rust).

## Test Status
![Full Test](https://github.com/LoliGothick/mitama-cpp-result/workflows/Full%20Test/badge.svg)

## References

[the User Reference](https://loligothick.github.io/mitama-cpp-result/).

If you need information on the nightly only API, please build the latest documentation in develop branch.

### Build the latest document (requires [poetry](https://python-poetry.org/))

```shell
mitama-cpp-result> $ cd docs
mitama-cpp-result/docs> $ poetry install --no-root
mitama-cpp-result/docs> $ poetry run mkdocs serve
```

## Dependencies

To use this library, you need at least 1.74.0 or higher version of the boost library.

## Supported OS / Compiler / Language Standard

- OS
    - Linux
    - Apple Darwin
    - (Windows)

- Compilers
    - gcc >= 11
    - clang >= 15
    - Latest preview version of MSVC only

- Language Standards
    - C++20
    - (MSVC: only support `/std:c++latest`)
