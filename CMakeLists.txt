cmake_minimum_required(VERSION 3.19)
project(TriggerBox)

set(CMAKE_CXX_STANDARD 14)

add_executable(TriggerBox
        main.cpp
        Spotters.cpp
        EventSource.cpp)

SET(OpenCV_DIR /usr/local/Cellar/opencv/4.5.2_4/lib/cmake/opencv4)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${OpenCV_LIBS})
