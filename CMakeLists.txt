cmake_minimum_required(VERSION 3.10)
project(UniDB)

set(CMAKE_CXX_STANDARD 17)

add_executable(UniDB
        main.cpp
        storage/ArrayCollection.cpp
        engine/ExecutionEngine.cpp
)

# هدرها در این مسیرها هستند
target_include_directories(UniDB PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/models
        ${CMAKE_SOURCE_DIR}/storage
        ${CMAKE_SOURCE_DIR}/engine
        ${CMAKE_SOURCE_DIR}/parser
)
