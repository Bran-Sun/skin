cmake_minimum_required(VERSION 3.6)
project(skin)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(skin ${SOURCE_FILES})
target_link_libraries(skin ${OpenCV_LIBS})