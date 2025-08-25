#include <solver.hpp>


Solver::Solver(Problem* problem):problem(problem){

}

Solver::~Solver(){
}

Solution Solver::solve(){
    PatternMerger patternMerger(problem);
    StagePatterns patterns=patternMerger.generate_patterns();

    PatternSelector patternSelector(*problem);
    patternSelector.partsnum_register(patterns);
    Scheme scheme=patternSelector.select();

    LNS lns(problem,scheme,patterns);
    lns.run();

    return Solution(*problem,patterns,lns.get_best());
}

Solutions Solver::solve_multi_solution_multi_thread(){
    Solutions result;
    PatternMerger patternMerger(problem);
    StagePatterns patterns=patternMerger.generate_patterns();

    size_t solutions_size=patterns.patterns.size()-1;
    result.solutions.resize(solutions_size);
    auto select_and_lns=[this,&patterns,&result](size_t max_stage){
        PatternSelector patternSelector(*problem);
        patternSelector.partsnum_register(patterns,max_stage);
        Scheme scheme=patternSelector.select();

        LNS lns(problem,scheme,patterns);
        lns.run();
        result.solutions[max_stage-1]=new Solution(*problem,patterns,lns.get_best());
    };
    std::vector<std::thread> threads;
    for(size_t max_stage=1;max_stage<patterns.patterns.size();max_stage++){
        threads.emplace_back(select_and_lns,max_stage);
    }
    for (auto& t : threads) {
        t.join();
    }
    return result;
}

Solutions Solver::solve_multi_solution_single_thread(){
    Solutions result;
    PatternMerger patternMerger(problem);
    StagePatterns patterns=patternMerger.generate_patterns();

    for(size_t max_stage=1;max_stage<patterns.patterns.size();max_stage++){
        PatternSelector patternSelector(*problem);
        patternSelector.partsnum_register(patterns,max_stage);
        Scheme scheme=patternSelector.select();

        LNS lns(problem,scheme,patterns);
        lns.run();
        result.push(new Solution(*problem,patterns,lns.get_best()));
        // logger.log_json("test"+std::to_string(max_stage),result.back().to_json());
    }

    return result;
}