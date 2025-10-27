#include "config.hpp"
#include "problem.hpp"
#include "solution.hpp"
#include "utils.hpp"
#include <solver.hpp>
#include <thread>

Solver::Solver(Problem* problem,SolverConfig config):problem(problem),config(config){
}

Solver::~Solver(){
}

Solution Solver::solve(){
    if(config.MODE==0 || config.MODE==2) return solve_normal();
    else if(config.MODE==1) return solve_with_pre_process();
    else throw cleanAndError("Solver::solve 非法MODE");
}

/// @brief 单线程求解
/// @return CSP问题的解
Solution Solver::solve_normal(){
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

Solution Solver::solve_with_pre_process(){
    problem->check_self();
    Timer timer(config.TIME_LIMIT);
    timer.print(Color::YELLOW,"[start solving]\n");
    timer.print(Color::PURPLE,config.general_to_string(),"\n");
    Solution result;
    timer.print(Color::YELLOW, "[开始预处理]\n");
    SolverConfig pre_config=config;
    pre_config.MAX_STAGE=1;
    pre_config.PREFER_BENCHMARK_MODE=1;
    timer.print(Color::YELLOW, "[开始预模式生成]\n");
    PatternMerger pre_merger(problem,timer,pre_config);
    StagePatterns pre_patterns=pre_merger.generate_patterns();
    Scheme pre_scheme=pre_patterns.get_order1_scheme();
    timer.print(Color::YELLOW, "[开始预LNS]\n");
    LNS pre_lns(problem,pre_scheme,pre_patterns,timer,pre_config);
    pre_lns.run(timer.get_remain_time()/4);
    Solution pre_solution(*problem,pre_patterns,pre_lns.get_best());
    pre_solution.check_self(timer);
    timer.print(Color::YELLOW, "[开始删除复杂方案]\n");
    pre_solution.delete_complex_node();
    PartsNum pre_parts_num=pre_solution.get_parts_num();
    timer.print(Color::YELLOW, "[保留",pre_parts_num.to_num(),"个零件]\n");
    result.merge(pre_solution);

    timer.print(Color::YELLOW, "[产生正式问题]\n");
    Problem mid_problem=problem->minus(pre_parts_num);
    timer.print(Color::YELLOW, "[开始正式模式生成]\n");
    PatternMerger patternMerger(&mid_problem,timer,config);
    StagePatterns patterns=patternMerger.generate_patterns();
    patterns.check_self();
    // time_limit=get_remain_time();
    timer.print(Color::YELLOW, "[开始正式模式选择]\n");
    Scheme scheme;
    if(config.ENABLE_SELECTOR){
        PatternSelector patternSelector(mid_problem,timer,config);
        patternSelector.partsnum_register(patterns);
        scheme=patternSelector.select();
    }
    timer.print(Color::YELLOW, "[开始正式LNS]\n");
    LNS lns(&mid_problem,scheme,patterns,timer,config);
    lns.run();
    lns.get_best().check_self(timer);
    timer.print(Color::YELLOW, "[开始正式获取解]\n");
    Solution lns_solution(mid_problem,patterns,lns.get_best());
    timer.print(Color::YELLOW, "[开始结合解]\n");
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