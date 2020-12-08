# mitama-cpp-result

**mitama-cpp-result** is the C++17 libraries for error handling without exceptions.

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

To use this library, you need at least 1.67.0 or higher version of the boost library.

## OS / Compiler / Language Standard

- support architectures
    - Linux
    - Apple Darwin
    - (Windows)

- support compilers
    - gcc >= 8.3
    - clang >= 7.0.1
    - Latest preview version of MSVC only 

- support language standards
    - C++17
    - C++20
    - (MSVC: only support `/std:c++latest`)
