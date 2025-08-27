#include <solver.hpp>
#include <visualizer.hpp>
#include <chrono>



int main(int argc, char** argv) {
    Timer timer;
    Logger logger("main");
    Problem problem_obj=Problem::from_json("../assets/test_problem.json");
    Problem* problem=&problem_obj;  
    read_config("../config.json");
    auto start = std::chrono::high_resolution_clock::now();
    PatternMerger patternMerger(problem,timer);
    StagePatterns patterns=patternMerger.generate_patterns();

    PatternSelector patternSelector(*problem,timer);
    patternSelector.partsnum_register(patterns);
    Scheme scheme=patternSelector.select();

    LNS lns(problem,scheme,patterns,timer);
    lns.run();
    logger.log_json("testLNSjson",patterns.to_json(lns.get_history().back()));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    for(auto solution:lns.lastProcess->history){
        logger.log_json("testProcess"+std::to_string(solution->placed_pattern()),patterns.to_json(solution));
    }

    Visualizer::init();
    Visualizer::from_history(&patterns,lns.get_history());
    std::cout<<Visualizer::layouts.size()<<std::endl;
    
    Visualizer::show();

    return 0;
}