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

#tests
set(TEST_MAIN_FUNC ./external/googletest/googletest/src/gtest_main.cc)
file(GLOB TEST_FILES ./tests/*.cpp)
foreach(file ${TEST_FILES})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file} ${TEST_MAIN_FUNC})
    target_link_libraries(${name} gtest cl_base cl_solver cl_visualizer cl_input)
endforeach()
add_executable(test_all ${TEST_MAIN_FUNC} ${TEST_FILES})
target_link_libraries(test_all gtest cl_base cl_solver cl_visualizer cl_input)
