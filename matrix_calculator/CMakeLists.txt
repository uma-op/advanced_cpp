cmake_minimum_required(VERSION 3.15)
project(matrix_calculator)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${INCLUDE_DIR})

add_custom_target(
    ${PROJECT_NAME}_TEST
    COMMAND ctest -VV ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${PROJECT_NAME}_tests
)

add_custom_target(
    ${PROJECT_NAME}_LINT
    COMMAND cpplint --filter=-legal/copyright,-runtime/explicit,-build/include_subdir --linelength=120 --recursive ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    ${PROJECT_NAME}_COVERAGE
    COMMAND echo "Not implemented"
)

enable_testing()

find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")

message("test sources is ${TEST_SOURCES}")

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} GTest::gtest GTest::gtest_main)

add_test(NAME do-some-test COMMAND ${PROJECT_NAME}_tests)