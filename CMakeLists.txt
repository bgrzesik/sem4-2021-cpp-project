cmake_minimum_required(VERSION 3.16)
project(TriggerBox)

set(CMAKE_CXX_STANDARD 20)

add_library(TriggerBox
        Constants.cpp
        Spotters.cpp
        PipelineContext.cpp
        FiltersNodes.cpp
        PipelineNode.cpp
        SingleThreadedContext.cpp
        MultiThreadedContext.cpp
        )

find_package(OpenCV "4.5.2" REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${OpenCV_LIBS})

find_package(Boost "1.67.0" REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${Boost_LIBS})

add_executable(Example_1
        main.cpp)

target_link_libraries(Example_1 TriggerBox)
