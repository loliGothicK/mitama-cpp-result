set(TEST_NAMES
        result_tests
        maybe_tests
        anyhow_tests
)

find_package(Threads REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

# Find Boost
find_package(Boost 1.67.0 REQUIRED)

message($CMAKE)

foreach(TEST_NAME ${TEST_NAMES})
  add_executable(${TEST_NAME} ${TEST_NAME}.cpp)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /Zc:__cplusplus /bigobj /W4 /EHsc")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /std:c++latest /Zc:__cplusplus /bigobj /Zi")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /std:c++latest /Zc:__cplusplus /bigobj /DEBUG /OPT:REF /OPT:ICF")

  target_include_directories(${TEST_NAME}
    PRIVATE ${Boost_INCLUDE_DIRS}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/Catch2/single_include
    PRIVATE ${CMAKE_SOURCE_DIR}/mitama-utest-utilities/include
  )

  target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES})
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach ()

enable_testing()
