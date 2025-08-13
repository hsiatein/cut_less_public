#include <process.hpp>

Process::Process(){

}

Process::Process(const Process& other):operations(other.operations){
    for(auto solution:other.history){
        history.push_back(new PatternSolution(*solution));
    }
}


Process::~Process(){
    for(auto solution:history){
        delete solution;
    }
}

void Process::log_solution(PatternSolution* solution){
    history.push_back(new PatternSolution(*solution));
}

void Process::log_option(Option option){
    operations.push_back(option);
}