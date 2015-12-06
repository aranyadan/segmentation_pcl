cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(paper)

find_package(PCL 1.2 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
add_definitions(${PCL_DEFINITIONS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable (harris src/harriscorner.cpp)
target_link_libraries (harris ${OpenCV_LIBRARIES} ${PCL_LIBRARIES})



