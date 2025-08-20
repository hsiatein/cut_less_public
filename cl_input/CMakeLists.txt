cmake_minimum_required(VERSION 3.12)
project(cl_input CXX)

file(GLOB SRCS ./src/*.cpp)
add_library(cl_input SHARED ${SRCS})
set_target_properties(cl_input PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(cl_input PUBLIC cxx_std_17)
target_include_directories(cl_input PUBLIC ./include)
target_link_libraries(cl_input cl_base xlnt)
target_compile_definitions(cl_input
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)
