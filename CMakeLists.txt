cmake_minimum_required(VERSION 3.12)
project(cut_less_copy CXX)
set(PROJECT_VERSION 0.1.0)
if (MSVC)
    add_compile_options(/utf-8)
endif()

add_subdirectory(./external/argparse)
add_subdirectory(./external/HiGHS)
add_subdirectory(./external/polyscope)
add_subdirectory(./cl_base/)
add_subdirectory(./cl_solver/)
add_subdirectory(./cl_visualizer/)

add_executable(main ./main.cpp)
target_compile_features(main PUBLIC cxx_std_17)
target_link_libraries(main cl_solver cl_visualizer argparse)

