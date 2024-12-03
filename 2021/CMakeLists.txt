cmake_minimum_required(VERSION 3.21)

project(aoc2021)

FILE(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR} **.cpp)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

find_package(fmt REQUIRED)

foreach(FILE IN LISTS SOURCES)
    cmake_path(GET FILE STEM FILE_NAME)
    message(STATUS "Adding executable: ${FILE_NAME} from ${FILE}")
    add_executable(${FILE_NAME} ${FILE})
    target_link_libraries(${FILE_NAME} PRIVATE fmt::fmt)
    target_compile_features(${FILE_NAME} PRIVATE cxx_std_20)
    cmake_path(GET FILE PARENT_PATH FILE_PARENT_PATH)
    message(STATUS "Using input file from ${FILE_PARENT_PATH}/input.txt")
    add_custom_command(TARGET ${FILE_NAME} POST_BUILD
        COMMAND ${FILE_NAME} ${FILE_PARENT_PATH}/input.txt  VERBATIM)
endforeach()
