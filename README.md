# mitama-cpp-result

**mitama-cpp-result** is the C++23 libraries for error handling without exceptions.

**mitama-cpp-result** provides `result<T, E>`, `maybe<T>`, and associated monadic functions
(like [Result](https://doc.rust-lang.org/std/result/enum.Result.html)
and [Option](https://doc.rust-lang.org/std/option/enum.Option.html) in Programming Language Rust).

## Test Status
[![Full Test](https://github.com/LoliGothick/mitama-cpp-result/actions/workflows/full-test.yml/badge.svg)](https://github.com/LoliGothick/mitama-cpp-result/actions/workflows/full-test.yml)

## Installation

Read [Installation](https://loligothick.github.io/mitama-cpp-result/#installation) in [the User Reference](https://loligothick.github.io/mitama-cpp-result/).

## Contributing

### Development

```shell
$ cmake -B build -DBUILD_TESTING=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
$ make -C build -j $(nproc)
$ ln -s $PWD/build/compile_commands.json $PWD
```

### Run tests

```shell
$ cmake -B build -DBUILD_TESTING=ON
$ make -C build -j $(nproc)
$ cd build
$ ctest --output-on-failure
```

### Run linter

```shell
$ cabin lint --exclude Catch2
```

### Run formatter

```shell
$ cabin fmt --exclude Catch2
```

### Build the latest document (requires [poetry](https://python-poetry.org/))

```shell
mitama-cpp-result> $ cd docs
mitama-cpp-result/docs> $ poetry install --no-root
mitama-cpp-result/docs> $ poetry run mkdocs serve
```
