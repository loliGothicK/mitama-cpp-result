# Contribution Guide

Any contribution to Mitama.Dimensional is more than welcome!

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
Initialize submodule with command as follows:

```shell
$ git sbumodule update --init --recursive
```

The library does not depend on Boost, but the tests use Boost.
Although we do not use the Boost library that needs to be built, we recommend that you install it so that CMake will automatically find it (Please refer to [Boost Wiki](https://github.com/boostorg/boost/wiki/Getting-Started)).

The other tool you need besides the C ++ compiler to test is CMake.
Please install CMake-3.1 or later which can specify C++17.


## Unit tests
-------------------

Test is located under `tests` directory in the repository root.

There is a CMakeLists.txt file in repository root.

Perform an outsource build as follows:

```shell
$ mkdir build
$ cd build
$ cmake ..
$ make -j10
$ ctest
```

## Examples
------------

If you are implementing a new feature, add sample code to `examples/`. This will help us understand the changes a lot and help Mitama.Dimensional users understand your well-organized features.


## Documentation
-----------------

If you add a new feature, you will probably want to change the document.
Please send PR to `docs` branch.
Of course, typo and minor wording fixes are also welcome.