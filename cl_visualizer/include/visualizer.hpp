#pragma once
#include <polyscope/polyscope.h>
#include <polyscope/curve_network.h>
#include <polyscope/surface_mesh.h>
#include "cube.hpp"
#include <stage_patterns.hpp>
#include <process.hpp>
#include <solution.hpp>


class cl_visualizer_EXPORT Visualizer{
public:

    static void init();
    static void show();
    static void from_history(StagePatterns* patterns,History& history);
    static void from_pattern_solution(StagePatterns* patterns,PatternSolution* solution);
    static void from_solution(Solution& solution);
    static void from_process(StagePatterns* patterns,Process* process);
    static void display(int i);
    static std::vector<Cube> layouts;
    static std::vector<polyscope::SurfaceMesh*> cuts;
    static int current_cut;
    static int current;
    static int current_solution;
    static StagePatterns* patterns;
    static History* history;
    static int history_length;
    static std::map<std::string,Vec3i> cubeStatus;

    
private:
    static std::pair<polyscope::SurfaceMesh*,polyscope::CurveNetwork*> add_cube(const Vec3i location,const Vec3i size);
    static void add_part(const Vec3i location,const Vec3i size);
    static void add_leftover(const Vec3i location,const Vec3i size);
    static void add_cutloss(const Vec3i location,const Vec3i size);
    static void from_cube(Cube& cube);

    static size_t num;
    const static std::vector<std::array<size_t, 3>> faces;
    const static std::vector<std::array<size_t, 2>> edges;

};