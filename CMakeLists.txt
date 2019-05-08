cmake_minimum_required(VERSION 3.1)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17) # C++17...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
find_package(Threads REQUIRED)

## Set our project name
project(mitama-cpp-result)

include_directories(
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Catch2/single_include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mitama-utest-utilities/include
  )

set(target tests)
set(sources
  test/Result_Test.cpp
)
add_executable(${target} ${sources})
message(STATUS "Building library ${target}...")

add_test(tests "${EXECUTABLE_OUTPUT_PATH}/${target}")
enable_testing()
