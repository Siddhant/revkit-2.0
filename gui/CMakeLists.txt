cmake_minimum_required(VERSION 2.8.11)
project(revkit_gui)

add_definitions(-std=c++11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex filesystem graph program_options system)

include_directories( ../src )
link_directories( ../build/src )

set(revkit_viewer_SRCS
    src/circuit_view.cpp
    src/truth_table_widget.cpp
    programs/viewer/viewer_window.cpp
    programs/viewer/main.cpp)

add_executable(revkit_viewer ${revkit_viewer_SRCS})
target_link_libraries(revkit_viewer Qt5::Gui Qt5::Widgets revkit_core_static revkit_reversible_static revkit_classical_static ${Boost_LIBRARIES})

