cmake_minimum_required(VERSION 3.20)
project(UniDB)

set(CMAKE_CXX_STANDARD 20)

# اضافه کردن پوشه‌های شامل
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/storage
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
        ${CMAKE_CURRENT_SOURCE_DIR}/parser
)

add_executable(UniDB main.cpp)