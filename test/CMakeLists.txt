find_package(Threads REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

# Find Boost
find_package(Boost 1.67.0 REQUIRED
  COMPONENTS
  stacktrace_addr2line
)

message($CMAKE)

add_executable(result-tests Result_Test.cpp)
add_executable(maybe-tests Maybe_Test.cpp)

target_compile_features(result-tests PRIVATE cxx_std_17)
target_compile_features(maybe-tests PRIVATE cxx_std_17)

target_compile_options(result-tests PRIVATE -Wall -Wextra -pedantic-errors -ftemplate-backtrace-limit=0)
target_compile_options(maybe-tests PRIVATE -Wall -Wextra -pedantic-errors -ftemplate-backtrace-limit=0)
# -Wall -Wextra -Werror -pedantic-errors

if(APPLE)
  target_compile_definitions(result-tests PRIVATE _GNU_SOURCE)
endif()

target_include_directories(result-tests
  PRIVATE ${Boost_INCLUDE_DIRS}
  PRIVATE ${CMAKE_SOURCE_DIR}/include
  PRIVATE ${CMAKE_SOURCE_DIR}/Catch2/single_include
  PRIVATE ${CMAKE_SOURCE_DIR}/mitama-utest-utilities/include
)
target_include_directories(maybe-tests
  PRIVATE ${Boost_INCLUDE_DIRS}
  PRIVATE ${CMAKE_SOURCE_DIR}/include
  PRIVATE ${CMAKE_SOURCE_DIR}/Catch2/single_include
  PRIVATE ${CMAKE_SOURCE_DIR}/mitama-utest-utilities/include
)

target_link_libraries(result-tests ${Boost_LIBRARIES} dl)
target_link_libraries(maybe-tests ${Boost_LIBRARIES} dl)

add_test(NAME result-tests COMMAND result-tests WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
add_test(NAME maybe-tests COMMAND maybe-tests WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
enable_testing()
