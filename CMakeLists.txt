cmake_minimum_required(VERSION 3.20)
project(AdvancedCpp)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)

set(HOMEWORK_DIRS matrix_calculator)
set(TARGET_TYPES TEST LINT)

foreach(HOMEWORK_DIR IN LISTS HOMEWORK_DIRS)
    add_subdirectory(${HOMEWORK_DIR})
    
    foreach(TARGET_TYPE IN LISTS TARGET_TYPES)
        list(APPEND HOMEWORK_${TARGET_TYPE}_TARGETS ${HOMEWORK_DIR}_${TARGET_TYPE})
    endforeach()
endforeach()

