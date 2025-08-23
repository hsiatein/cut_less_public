cmake_minimum_required(VERSION 3.12)
project(cl_base CXX)

file(GLOB SRCS ./src/*.cpp)
add_library(cl_base STATIC ${SRCS})
set_target_properties(cl_base PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(cl_base PUBLIC cxx_std_17)
target_include_directories(cl_base PUBLIC ./include)
target_link_libraries(cl_base)
target_compile_definitions(cl_base
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)
