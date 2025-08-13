#include <solver.hpp>


Solver::Solver(Problem* problem):problem(problem),solution(nullptr){

}

Solver::~Solver(){
    if(solution!=nullptr) delete solution;
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
    solution=new PatternSolution(lns.get_best());

    return Solution(*problem,patterns,*solution);
}



