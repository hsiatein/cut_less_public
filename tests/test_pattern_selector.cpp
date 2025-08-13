#include "gtest/gtest.h"
#include <solver.hpp>
#include <visualizer.hpp>

namespace test_pattern_selector{


Logger logger("test_pattern_selector");
Problem problem=Problem::from_json("../assets/test_problem.json");
// TEST(PatternSelector,init){
//     read_config("../config.json");
//     problem=Problem::from_json("../assets/test_problem.json");
// }



// TEST(PatternSelector,testRegister){
//     Solver solver(&problem);
//     solver.generate_patterns();
//     solver.patternSelector.partsnum_register(solver.patterns);
//     // std::cout<<1<<std::endl;
//     logger.log_json("testRegister",solver.patternSelector.to_json());
//     // std::cout<<1<<std::endl;
//     logger.log(std::to_string(solver.patternSelector.get_patterns_size()));
//     // std::cout<<1<<std::endl;
//     logger.log(std::to_string(solver.patternSelector.partsnums.size()));
//     // std::cout<<1<<std::endl;

// }

// TEST(PatternSelector,testSelect){
//     Solver solver(&problem);
//     solver.generate_patterns();
//     solver.patternSelector.partsnum_register(solver.patterns);
//     Scheme scheme=solver.patternSelector.select();
//     logger.log_json("testScheme",solver.patternSelector.scheme_to_json(scheme));
//     LNS lns(&problem,scheme,solver.patterns);
//     lns.run();
//     logger.log_json("testLNSjson",lns.to_json(lns.get_history().back()));
//     // solver.solve();

// }


}