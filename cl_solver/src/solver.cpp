#include "config.hpp"
#include "solution.hpp"
#include <solver.hpp>
#include <thread>

Solver::Solver(Problem* problem,SolverConfig config):problem(problem),config(config){
}

Solver::~Solver(){
}

/// @brief 单线程求解
/// @return CSP问题的解
Solution Solver::solve(){
    problem->check_self();
    Timer timer(config.TIME_LIMIT);
    timer.print(Color::YELLOW,"[start solving]\n");
    timer.print(Color::PURPLE,config.general_to_string(),"\n");
    Solution result;

    PatternMerger patternMerger(problem,timer,config);
    StagePatterns patterns=patternMerger.generate_patterns();
    patterns.check_self();
    // time_limit=get_remain_time();

    Scheme scheme;
    if(config.ENABLE_SELECTOR){
        PatternSelector patternSelector(*problem,timer,config);
        patternSelector.partsnum_register(patterns);
        scheme=patternSelector.select();
    }
    
    LNS lns(problem,scheme,patterns,timer,config);
    lns.run();
    lns.get_best().check_self(timer);
    Solution lns_solution(*problem,patterns,lns.get_best());
    result.merge(lns_solution);
    result.check_self(timer);

    timer.print(Color::YELLOW,"[Execution time: ",timer.get_runtime()," ms]\n");
    return result;
}

/// @brief 多线程求多个解
/// @return CSP问题的多个解
Solutions Solver::solve_multi_solution_multi_thread(){
    Timer timer(config.TIME_LIMIT);
    Solutions result;
    PatternMerger patternMerger(problem,timer,config);
    StagePatterns patterns=patternMerger.generate_patterns();

    size_t solutions_size=patterns.patterns.size()-1;
    result.solutions.resize(solutions_size);
    auto select_and_lns=[this,&patterns,&result,&timer](size_t max_stage){
        PatternSelector patternSelector(*problem,timer,config);
        patternSelector.partsnum_register(patterns,max_stage);
        Scheme scheme=patternSelector.select();

        LNS lns(problem,scheme,patterns,timer,config);
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
    Timer timer(config.TIME_LIMIT);
    Solutions result;
    PatternMerger patternMerger(problem,timer,config);
    StagePatterns patterns=patternMerger.generate_patterns();

    for(size_t max_stage=1;max_stage<patterns.patterns.size();max_stage++){
        PatternSelector patternSelector(*problem,timer,config);
        patternSelector.partsnum_register(patterns,max_stage);
        Scheme scheme=patternSelector.select();

        LNS lns(problem,scheme,patterns,timer,config);
        lns.run();
        result.push(new Solution(*problem,patterns,lns.get_best()));
        // logger.log_json("test"+std::to_string(max_stage),result.back().to_json());
    }

    return result;
}