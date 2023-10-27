cmake_minimum_required(VERSION 3.14)
project(MusicIndexer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
string(CONCAT CMAKE_CXX_FLAGS
             "${CMAKE_CXX_FLAGS}"
             " -Wall -Wextra"
             " -Wfloat-conversion -Wsign-conversion"
             " -Wshadow -Wduplicated-cond"
             " -Wduplicated-branches -Wlogical-op"
             " -Wnull-dereference -Wuseless-cast"
             " -Wformat=2")

option(BUILD_TESTS "Whether to build tests" FALSE)

set(BUILD_TESTING OFF)

# SqliteCpp
find_package(SQLiteCpp REQUIRED)

# TagLib
find_package(taglib)

add_library(MusicIndexer STATIC src/music_indexer.cpp)

include_directories(include)

target_link_libraries(MusicIndexer
   SQLiteCpp
   tag
)

if (BUILD_TESTS)
   add_executable(test src/main.cpp)
   target_link_libraries(test MusicIndexer)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Copy root/build/compile_commands.json to root/
if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
   FILE(COPY ${CMAKE_BINARY_DIR}/compile_commands.json DESTINATION ${CMAKE_HOME_DIRECTORY})
endif()
