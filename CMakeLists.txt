cmake_minimum_required(VERSION 3.14)
project(MuCpp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
string(CONCAT CMAKE_CXX_FLAGS
             "${CMAKE_CXX_FLAGS}"
             " -Wall -Wextra"
             " -Wfloat-conversion -Wsign-conversion"
             " -Wshadow -Wduplicated-cond"
             " -Wduplicated-branches -Wlogical-op"
             " -Wnull-dereference -Wuseless-cast"
             " -Wformat=2")

find_package(Qt6 REQUIRED COMPONENTS Widgets UiTools Multimedia)
qt_standard_project_setup()

add_subdirectory("deps/Music-Indexer")

add_executable(mucpp
  src/clickable_label.cpp
  src/album_widget.cpp
  src/flow_layout.cpp
  src/mucpp.cpp
  src/main.cpp)

target_link_libraries(mucpp
  MusicIndexer
  Qt6::Widgets
  Qt6::Multimedia
  Qt6::UiTools
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Copy root/build/compile_commands.json to the root directory of the project
if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
   FILE(COPY ${CMAKE_BINARY_DIR}/compile_commands.json DESTINATION ${CMAKE_HOME_DIRECTORY})
endif()
