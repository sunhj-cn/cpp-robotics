cmake_minimum_required(VERSION 3.1)
project(CPP_ROBOTICS)
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#############################
#Dependencies
#############################
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
set(EIGEN3_INCLUDE_DIR "C:\\Program Files (x86)\\Eigen3\\include\\eigen")

include_directories(${EIGEN3_INCLUDE_DIR})
message("EIGEN3_INCLUDE_DIR path is: ${EIGEN3_INCLUDE_DIR}")
include_directories(./include)
include_directories(./include/eigen3)

##############################
#PathPlanning
##############################

add_executable(my_dijkstra src/my_dijkstra.cpp)
target_link_libraries(my_dijkstra ${OpenCV_LIBS} )