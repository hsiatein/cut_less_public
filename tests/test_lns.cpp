#include "gtest/gtest.h"
#include "solver.hpp"
#include <visualizer.hpp>
#include <chrono>

namespace test_lns{



// TEST(LNS,testLNS){
//     // Logger logger("test_lns");
//     auto start = std::chrono::high_resolution_clock::now();
//     Problem problem=Problem::from_json("../assets/test_problem2.json");
//     read_config("../assets/config.json");
//     PatternMerger patternMerger(&problem);
//     StagePatterns patterns=patternMerger.generate_patterns();

//     PatternSelector patternSelector(&problem);
//     patternSelector.partsnum_register(patterns);
//     Scheme scheme=patternSelector.select();

//     LNS lns(&problem,scheme,patterns);
//     lns.run();
//     logger.log_json("testLNSjson",patterns.to_json(lns.get_history().back()));
//     // solver.solve();
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double, std::milli> duration = end - start;
//     std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

//     for(auto solution:lns.lastProcess->history){
//         logger.log_json("testProcess"+std::to_string(solution->placed_pattern()),patterns.to_json(solution));
//     }

//     Visualizer::init();
//     Visualizer::from_process(&patterns,lns.lastProcess);
//     // Visualizer::from_history(&lns,lns.get_history());
//     std::cout<<Visualizer::layouts.size()<<std::endl;
    
//     Visualizer::show();

// }

}