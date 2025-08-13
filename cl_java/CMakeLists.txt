cmake_minimum_required(VERSION 3.12)
project(cl_java CXX)

file(GLOB SRCS ./src/*.cpp)
add_library(cl_java SHARED ${SRCS})
set_target_properties(cl_java PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(cl_java PUBLIC cxx_std_17)
target_include_directories(cl_java PUBLIC ./include)
target_link_libraries(cl_java cl_solver cl_base)
