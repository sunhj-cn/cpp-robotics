cmake_minimum_required(VERSION 3.1)
project(CPP_ROBOTICS)
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)

add_executable(my_dijkstra src/my_dijkstra.cpp)
target_link_libraries(my_dijkstra ${OpenCV_LIBS} )