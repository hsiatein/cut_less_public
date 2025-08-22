#include <solver.hpp>


Solver::Solver(Problem* problem):problem(problem){

}

Solver::~Solver(){
}

Solution Solver::solve(){
    logger.enableTXT=false;
    PatternMerger patternMerger(problem);
    StagePatterns patterns=patternMerger.generate_patterns();

    PatternSelector patternSelector(problem);
    patternSelector.partsnum_register(patterns);
    Scheme scheme=patternSelector.select();

    LNS lns(problem,scheme,patterns);
    lns.run();

    return Solution(*problem,patterns,lns.get_best());
}

std::vector<Solution> Solver::solve_multi_solution(){
    std::vector<Solution> result;
    logger.enableTXT=false;
    PatternMerger patternMerger(problem);
    StagePatterns patterns=patternMerger.generate_patterns();

    for(size_t max_stage=1;max_stage<patterns.patterns.size();max_stage++){
        PatternSelector patternSelector(problem);
        patternSelector.partsnum_register(patterns,max_stage);
        Scheme scheme=patternSelector.select();

        LNS lns(problem,scheme,patterns);
        lns.run();
        result.emplace_back(*problem,patterns,lns.get_best());
        // logger.log_json("test"+std::to_string(max_stage),result.back().to_json());
    }

    return result;
}

