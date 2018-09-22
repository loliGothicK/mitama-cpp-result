cmake_minimum_required(VERSION 2.8)
project(result VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

project(MitamaCppResult)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
enable_testing()

set(target result-test)
message(STATUS "Building library ${target}")

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/googletest
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Default include path at src/
include_directories(
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

# EXECUTABLE: result-test
add_executable(result-test
  test/Result_Test.cpp
  )
target_link_libraries(result-test
  gtest
  gtest_main
  pthread
  )
add_test(result "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(result PROPERTIES LABELS "all;result")

