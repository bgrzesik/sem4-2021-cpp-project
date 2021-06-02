cmake_minimum_required(VERSION 3.16)
project(TriggerBox)

set(CMAKE_CXX_STANDARD 14)

add_executable(TriggerBox
        main.cpp
        Spotters.cpp
        EventSource.cpp)


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${OpenCV_LIBS})
