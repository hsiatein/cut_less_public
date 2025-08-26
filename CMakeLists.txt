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
target_compile_definitions(main
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)

add_executable(main_json ./main_json.cpp)
target_compile_features(main_json PUBLIC cxx_std_17)
target_link_libraries(main_json cl_solver cl_visualizer argparse)
target_compile_definitions(main_json
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)

add_executable(main_multi_solution ./main_multi_solution.cpp)
target_compile_features(main_multi_solution PUBLIC cxx_std_17)
target_link_libraries(main_multi_solution cl_solver cl_visualizer argparse)
target_compile_definitions(main_multi_solution
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)

add_executable(visualize ./visualize.cpp)
target_compile_features(visualize PUBLIC cxx_std_17)
target_link_libraries(visualize cl_solver cl_visualizer)
target_compile_definitions(visualize
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
)

