cmake_minimum_required(VERSION 3.16)
project(TriggerBox)

set(CMAKE_CXX_STANDARD 20)

add_executable(TriggerBox
        main.cpp
        Constants.cpp
        Spotters.cpp
        Pipeline.cpp
        FiltersNodes.cpp
        PipelineNode.cpp
        )


find_package(OpenCV "4.5.2" REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${OpenCV_LIBS})

find_package(Boost "1.67.0" REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${Boost_LIBS})
