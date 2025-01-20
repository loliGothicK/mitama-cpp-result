set(TEST_NAMES
        result_tests
        maybe_tests
        anyhow_tests
)

set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

find_package(fmt 9.0.0 CONFIG REQUIRED)

message($CMAKE)

foreach(TEST_NAME ${TEST_NAMES})
  add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
  target_compile_features(${TEST_NAME} PRIVATE cxx_std_20)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -ftemplate-backtrace-limit=0 -fno-rtti)
  else()
    target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -ftemplate-backtrace-limit=0 -fno-rtti -pedantic-errors)
  endif()

  if(APPLE)
    target_compile_definitions(${TEST_NAME} PRIVATE _GNU_SOURCE)
  endif()

  target_include_directories(${TEST_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/Catch2/single_include
    PRIVATE ${CMAKE_SOURCE_DIR}/mitama-utest-utilities/include
  )

  target_link_libraries(${TEST_NAME} fmt::fmt-header-only)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach ()

enable_testing()
