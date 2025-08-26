#include <solver.hpp>


Solver::Solver(Problem* problem):problem(problem){

}

Solver::~Solver(){
}

/// @brief 单线程求解
/// @return CSP问题的解
Solution Solver::solve(){
    Timer timer;
    timer.print(Color::YELLOW,"[start solving]\n");
    Solution result;

    PatternMerger patternMerger(problem,timer);
    StagePatterns patterns=patternMerger.generate_patterns();
    // time_limit=get_remain_time();


    PatternSelector patternSelector(*problem,timer);
    patternSelector.partsnum_register(patterns);
    Scheme scheme=patternSelector.select();

    LNS lns(problem,scheme,patterns,timer);
    lns.run();
    Solution lns_solution(*problem,patterns,lns.get_best());
    result.merge(lns_solution);

    timer.print(Color::YELLOW,"[Execution time: ",timer.get_runtime()," ms]\n");
    return result;
}

/// @brief 多线程求多个解
/// @return CSP问题的多个解
Solutions Solver::solve_multi_solution_multi_thread(){
    Timer timer;
    Solutions result;
    PatternMerger patternMerger(problem,timer);
    StagePatterns patterns=patternMerger.generate_patterns();

    size_t solutions_size=patterns.patterns.size()-1;
    result.solutions.resize(solutions_size);
    auto select_and_lns=[this,&patterns,&result,&timer](size_t max_stage){
        PatternSelector patternSelector(*problem,timer);
        patternSelector.partsnum_register(patterns,max_stage);
        Scheme scheme=patternSelector.select();

        LNS lns(problem,scheme,patterns,timer);
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

/// @brief 单线程求多个解
/// @return CSP问题的多个解
Solutions Solver::solve_multi_solution_single_thread(){
    Timer timer;
    Solutions result;
    PatternMerger patternMerger(problem,timer);
    StagePatterns patterns=patternMerger.generate_patterns();

    for(size_t max_stage=1;max_stage<patterns.patterns.size();max_stage++){
        PatternSelector patternSelector(*problem,timer);
        patternSelector.partsnum_register(patterns,max_stage);
        Scheme scheme=patternSelector.select();

        LNS lns(problem,scheme,patterns,timer);
        lns.run();
        result.push(new Solution(*problem,patterns,lns.get_best()));
        // logger.log_json("test"+std::to_string(max_stage),result.back().to_json());
    }

    return result;
}