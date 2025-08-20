cmake_minimum_required(VERSION 3.12)
project(cl_solver CXX)

file(GLOB SRCS ./src/*.cpp)
add_library(cl_solver SHARED ${SRCS})
set_target_properties(cl_solver PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(cl_solver PUBLIC cxx_std_17)
target_include_directories(cl_solver PUBLIC ./include)
target_link_libraries(cl_solver highs cl_base)
target_compile_definitions(cl_solver
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)
