set(TEST_NAMES
        result_tests
        maybe_tests
        anyhow_tests
)

find_package(Threads REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

# Find Boost
find_package(Boost 1.67.0 REQUIRED
  COMPONENTS
  stacktrace_addr2line
)

# Find fmt
find_package(fmt CONFIG REQUIRED)

message($CMAKE)

foreach(TEST_NAME ${TEST_NAMES})
  add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
  target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -ftemplate-backtrace-limit=0)
  else()
    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -pedantic-errors -ftemplate-backtrace-limit=0)
  endif()

  if(APPLE)
    target_compile_definitions(${TEST_NAME} PRIVATE _GNU_SOURCE)
  endif()

  target_include_directories(${TEST_NAME}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/Catch2/single_include
    PRIVATE ${CMAKE_SOURCE_DIR}/mitama-utest-utilities/include
  )

  target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES} dl fmt::fmt-header-only)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach ()

enable_testing()
