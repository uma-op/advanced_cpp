cmake_minimum_required(VERSION 3.15)
project(matrix_calculator)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

add_custom_target(
    ${PROJECT_NAME}_TEST
    COMMAND echo "Test is not implemented"
)

add_custom_target(
    ${PROJECT_NAME}_LINT
    COMMAND cpplint --verbose=0123 --recursive ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    ${PROJECT_NAME}_COVERAGE
    COMMAND echo "Not implemented"
)