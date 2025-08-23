cmake_minimum_required(VERSION 3.12)
project(cl_visualizer CXX)

file(GLOB SRCS ./src/*.cpp)
add_library(cl_visualizer STATIC ${SRCS})
set_target_properties(cl_visualizer PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(cl_visualizer PUBLIC cxx_std_17)
target_include_directories(cl_visualizer PUBLIC ./include)
target_link_libraries(cl_visualizer polyscope cl_base)
target_compile_definitions(cl_visualizer
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)
