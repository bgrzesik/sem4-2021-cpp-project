


add_library(TriggerBox
        tb/Constants.cpp
        tb/Spotters.cpp
        tb/PipelineContext.cpp
        tb/FiltersNodes.cpp
        tb/PipelineNode.cpp
        tb/SingleThreadedContext.cpp
        tb/MultiThreadedContext.cpp
        tb/Notifier.cpp
        tb/DebugDraw.cpp
        tb/FrameSource.cpp
        )

find_package(OpenCV "4.5.2" REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${OpenCV_LIBS})

find_package(Boost "1.67.0" REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(TriggerBox ${Boost_LIBS})

target_include_directories(TriggerBox PUBLIC
                            .
                            ${Boost_INCLUDE_DIRS}
                            ${OpenCV_LIBS})