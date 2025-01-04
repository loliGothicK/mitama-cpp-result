# Contribution Guide

Any contribution to Mitama.Result is more than welcome!

## Reporting Issues
-------------------

The best way to contribute to this project is to send a detailed report when you encounter a problem.

It is preferable to minimize the code as much as possible in problematic situations.
Then include in the report the error code and environment (OS, compiler and its version, compile options) and the error message without omission.

## Pull request
---------------
Pull requests are welcome, even if they are just a typo fix!

However, any significant improvements should be related to the existing feature request or bug report.

## Start
----------

Clone this repository locally.
Initialize submodules with command as follows:

```shell
$ git sbumodule update --init --recursive
```

The other tool you need besides the C ++ compiler to test is CMake.
Please install CMake-3.1 or later which can specify C++17.


## Unit tests
-------------------

Test is located under `test` directory in the repository root.

There is a CMakeLists.txt file in repository root.

Perform an outsource build as follows:

```shell
$ mkdir build
$ cd build
$ cmake .. -DBUILD_TESTING=ON
$ make -j10
$ ctest
```

## Documentation
-----------------

If you add a new feature, you will probably want to change the document.
Please send PR to `docs` directory.
Of course, typo and minor wording fixes are also welcome.

## Document Test
-----------------

Integrated by GitHub Actions on every push.
